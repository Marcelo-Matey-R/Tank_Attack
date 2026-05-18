#include "Map.h"
#include "Directions.h"
#include "Pathfindings.h"
#include "EnumTypePath.h"

#pragma region Constructos and Destructor

Map::Map(){
    tanksPlayer0 = new Tank*[2];
    tanksPlayer1 = new Tank*[2];
}

Map::~Map(){
    if(tanksPlayer0 != nullptr){
        delete tanksPlayer0[0], tanksPlayer0[1], tanksPlayer0;
    }
	if(tanksPlayer1 != nullptr){
        delete tanksPlayer1[0], tanksPlayer1[1], tanksPlayer1;
    }
	for(int i = 0; i < GRID_ROWS; i++){
		for(int j = 0; j < GRID_COLS; j++){
			if(map[i][j] != nullptr) delete map[i][j];
		}
	}
	delete map;
}

#pragma endregion

#pragma region Create Map

void Map::FillMap(){
    SetTankOfPlayer1();
    SetTankOfPlayer2();

    for(int i = 0; i < GRID_ROWS; i++){
        for(int j = 0; j < GRID_COLS; j++){
            if(map[i][j] != nullptr) continue;

            int val = binary(gen);
            if(val == 0){
                map[i][j] = new Floor(false);
            }
            else map[i][j] = new Obstacle();
        }
    }

}

void Map::SetTankOfPlayer1(){
    int i = disRows(gen);
    int j = disCols(gen);

    Position pos = {i, j};

    Tank* tankBlue = new TankLightBlueAndBlue(ColorTank::BLUE);
    tanksPlayer0[0] = tankBlue;
    tankBlue->SetPosition(pos);

    Tank* tankRed = new TankYellowAndRed(ColorTank::RED);
    tanksPlayer0[1] = tankRed;

    while(IsNullPlace(pos)){
        pos.i = disRows(gen);
        pos.j = disCols(gen);
    }

    tankRed->SetPosition(pos);

    map[i][j] = tankRed;
}

void Map::SetTankOfPlayer2(){
    int i = disRows(gen);
    int j = disCols(gen);

    Position pos = {i, j};

    Tank* tankLightBlue = new TankLightBlueAndBlue(ColorTank::LIGTHBLUE);
    tanksPlayer0[0] = tankLightBlue;
    tankLightBlue->SetPosition(pos);

    Tank* tankYellow = new TankYellowAndRed(ColorTank::YELLOW);
    tanksPlayer1[1] = tankYellow;

    while(IsNullPlace(pos)){
        pos.i = disRows(gen);
        pos.j = disCols(gen);
    }

    pos = {i, j};
    tankYellow->SetPosition(pos);

    map[i][j] = tankYellow;
}

bool Map::VerifyConectivity(const Position destinies[4]){
    return Pathfinding::IsConected(map, destinies);
}

void Map::EliminateObstacles(const std::vector<Position> &pos){
    for(Position p : pos){
        EliminateObstacle(p);
    }
}

void Map::CreateConectivity(){
    Position destinies[4] = {tanksPlayer0[0]->GetPosition(), tanksPlayer0[1]->GetPosition(), tanksPlayer1[0]->GetPosition(), tanksPlayer1[1]->GetPosition()};
    if(VerifyConectivity(destinies)) return;
    for(int i = 0; i < 4; i++){
        for(int j = i+1; j < 4; j++){
            EliminateObstacles(Pathfinding::SelectAlgorithm(TypePath::ASTAR_MAP, map, destinies[i], destinies[j]));
        }
    }
}

#pragma endregion

#pragma region Helpers

bool Map::IsFLoor(const Position &pos){
	if(map[pos.i][pos.j]->GetType() != TypeElement::Floor) return false;
	return true;
}

bool Map::IsObstacle(const Position &pos){
	if(map[pos.i][pos.j]->GetType() != TypeElement::Obstacle) return false;
	return true;
} 

bool Map::IsTank(const Position &pos){
	if(map[pos.i][pos.j]->GetType() != TypeElement::Tank) return false;
	return true;
}

bool Map::IsNullPlace(const Position &pos){
    if(map[pos.i][pos.j] == nullptr) return true;
    return false;
}

bool Map::IsPositionOcupied(const Position &pos){
    return !IsFLoor(pos) && !IsNullPlace(pos);
}
	
bool Map::IsRealPosition(const Position &pos){
	return (pos.i >= 0 && pos.i < GRID_ROWS && pos.j >= 0 && pos.j < GRID_COLS);
}

bool Map::IsRealPlayer(int id){return (id == 0 || id == 1);}

void Map::EliminateObstacle(const Position &pos){
	if(!IsRealPosition(pos)){
    throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if(!IsObstacle(pos)){
        throw std::invalid_argument("La posicion no corresponde a un obstaculo");
    }

    delete map[pos.i][pos.j];
    map[pos.i][pos.j] = new Floor(false);
}

void Map::EliminateTank(Tank* tank, const Position &pos){
    if(!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if(!IsTank(pos)){
        throw std::invalid_argument("La posicion no corresponde a un tanque");
    }
    if(tank->IsAlive()){
        throw std::invalid_argument("No se puede eliminar un tanque vivo");
    }

    int player = -1;
    int idx = -1;

    for(int i = 0; i < 2; i++){
        if(tanksPlayer0[i] == tank){
            player = 0;
            int idx = i; 
        }
        else if(tanksPlayer1[i] == tank){
            player = 1;
            idx = i;
        }
    }

    if(player == -1 || idx == -1){
        throw std::invalid_argument("El tanque no existe en el mapa ni en la lista de jugadores");
    }

    if(player == 0) tanksPlayer0[idx] = nullptr;
    else tanksPlayer1[idx] = nullptr;

    delete map[pos.i][pos.j];
    map[pos.i][pos.j] = new Floor(true);

}

void Map::MoveTank(Tank* tank, Position &destination){
    if(!IsRealPosition(destination)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if(tank->IsAlive()){
        throw std::invalid_argument("No se puede eliminar un tanque vivo");
    }

    if(IsPositionOcupied(destination)){
        Position t = tank->GetPosition();
        Position newDestiny;
        int cost = INF;
        for(auto direction : allDirections){
            Position tmp = destination + direction;
            if(!IsRealPosition(tmp) || IsPositionOcupied(tmp)) continue;
            
            int dx = abs(t.i - tmp.i);
            int dy = abs(t.j - tmp.j);
            if((dx + dy) < cost){
                cost = dx + dy;
                newDestiny = tmp;
            }
        }

        if(cost == INF){
            throw std::invalid_argument("No se permiten destinos previamente ocupados");
        }

        destination = newDestiny;

    }
    Position pos = tank->GetPosition();
    map[pos.i][pos.j] = new Floor(false);
    tank->SetPosition(destination);
    map[destination.i][destination.j] = tank;

}

#pragma endregion

#pragma region Gets

Element* Map::GetElementAt(const Position &pos){
    if(!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    return map[pos.i][pos.j];
} 

Tank* Map::GetTankIn(const Position &pos){
    if(!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if(!IsTank(pos)){
        return nullptr;
    }
    return static_cast<Tank*>(map[pos.i][pos.j]);
} 

Tank* Map::GetTankOfPlayer(int idPlayer, ColorTank color){
    if(!IsRealPlayer(idPlayer)){
        throw std::invalid_argument("El id del jugador no existe");
    }
    switch (idPlayer)
    {
        case 0:
            if(color == ColorTank::BLUE) return tanksPlayer0[1];
            return tanksPlayer0[0];
        
        case 1:
            if(color == ColorTank::LIGTHBLUE) return tanksPlayer1[1];
            return tanksPlayer1[0];
        
        default:
            return nullptr;
    }
} 

Tank** Map::GetTanksOfPlayer(int idPlayer){
    if(!IsRealPlayer(idPlayer)){
        throw std::invalid_argument("El id del jugador no existe");
    }

    if(idPlayer == 0) return tanksPlayer0;

    return tanksPlayer1;
}

#pragma endregion
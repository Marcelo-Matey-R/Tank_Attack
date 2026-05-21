#include "Map.h"
#include "Directions.h"
#include "../include/Pathfindings.h"
#include "EnumTypePath.h"
#include <iostream>

#pragma region Constructos and Destructor

Map::Map(){
    tanksPlayer0 = nullptr;
    tanksPlayer1 = nullptr;
    InitMap();
}

void Map::InitMap(){
    if (tanksPlayer0 != nullptr || tanksPlayer1 != nullptr){
        std::cout << "Warning: InitMap llamado sin ResetMap previo" << std::endl;
        ResetMap();
    }
    std::cout << "Creando arrays de tanques..." << std::endl;
    tanksPlayer0 = new Tank *[4];
    tanksPlayer1 = new Tank *[4];

    std::cout << "Inicializando arrays a nullptr..." << std::endl;
    for (int i = 0; i < 4; i++){
        tanksPlayer0[i] = nullptr;
        tanksPlayer1[i] = nullptr;
    }

    std::cout << "Inicializando mapa a nullptr..." << std::endl;
    for (int i = 0; i < GRID_ROWS; i++)
        for (int j = 0; j < GRID_COLS; j++){
            if(map[i][j] == nullptr) continue;
            map[i][j] = nullptr;
        }

    std::cout << "Llamando FillMap..." << std::endl;
    FillMap();

    std::cout << "Llamando CreateConectivity..." << std::endl;
    CreateConectivity();

    std::cout << "Constructor completado" << std::endl;
}

void Map::ResetMap(){
    for (int i = 0; i < GRID_ROWS; i++){
        for (int j = 0; j < GRID_COLS; j++){
            if (map[i][j] != nullptr && map[i][j]->GetType() == TypeElement::Tank){
                map[i][j] = nullptr;
            }
        }
    }

    if (tanksPlayer0 != nullptr){
        for(int i = 0; i < 4; i++){
            if(tanksPlayer0[i] == nullptr) continue;
            delete tanksPlayer0[i];
            tanksPlayer0[i] = nullptr;
        }
        delete[] tanksPlayer0;  // ← delete[] para arrays
        tanksPlayer0 = nullptr;
    }
    if (tanksPlayer1 != nullptr){
        for(int i = 0; i < 4; i++){
            if(tanksPlayer1[i] == nullptr) continue;
            delete tanksPlayer1[i];
            tanksPlayer1[i] = nullptr;
        }
        delete[] tanksPlayer1;
        tanksPlayer1 = nullptr;
    }
    for (int i = 0; i < GRID_ROWS; i++){
        for (int j = 0; j < GRID_COLS; j++){
            if (map[i][j] != nullptr){
                delete map[i][j];
                map[i][j] = nullptr;
            }
        }
    }
}

Map::~Map(){
    ResetMap();
}

#pragma endregion

#pragma region Create Map

void Map::FillMap(){
    std::cout << "SetTankOfPlayer1..." << std::endl;
    SetTankOfPlayer1();
    std::cout << "SetTankOfPlayer2..." << std::endl;
    SetTankOfPlayer2();
    std::cout << "Llenando mapa con obstaculos..." << std::endl;
    for (int i = 0; i < GRID_ROWS; i++){
        for (int j = 0; j < GRID_COLS; j++){
            if (map[i][j] != nullptr)
                continue;
            int val = binary(gen);
            if (val == 0)
                map[i][j] = new Floor(false);
            else
                map[i][j] = new Obstacle();
        }
    }
    std::cout << "FillMap completado" << std::endl;
}

void Map::SpawnTank(Tank* tank, int idx, int idPlayer){
    Position pos;
    do{
        pos.i = disRows(gen);
        pos.j = disCols(gen);
    }while(!IsNullPlace(pos));

    tank->SetPosition(pos);
    map[pos.i][pos.j] = tank;

    if(idPlayer == 0){
        tanksPlayer0[idx] = tank;
    }
    else tanksPlayer1[idx] = tank;
}


void Map::SetTankOfPlayer1()
{
    SpawnTank(new TankLightBlueAndBlue(ColorTank::BLUE), 0, 0);
    SpawnTank(new TankLightBlueAndBlue(ColorTank::BLUE), 1, 0);
    SpawnTank(new TankYellowAndRed(ColorTank::RED), 2, 0);
    SpawnTank(new TankYellowAndRed(ColorTank::RED), 3, 0);
}

void Map::SetTankOfPlayer2(){
    SpawnTank(new TankLightBlueAndBlue(ColorTank::LIGTHBLUE), 0, 1);
    SpawnTank(new TankLightBlueAndBlue(ColorTank::LIGTHBLUE), 1, 1);
    SpawnTank(new TankYellowAndRed(ColorTank::YELLOW), 2, 1);
    SpawnTank(new TankYellowAndRed(ColorTank::YELLOW), 3, 1);
}

void Map::RestartGame(){
    ResetMap();
    InitMap();
}

bool Map::VerifyConectivity(const Position destinies[8]){
    return Pathfinding::IsConected(map, destinies);
}

void Map::EliminateObstacles(const std::vector<Position> &pos){
    for (Position p : pos){
        EliminateObstacle(p);
    }
}

void Map::CreateConectivity(){
    Position destinies[8] = {
        tanksPlayer0[0]->GetPosition(),
        tanksPlayer0[1]->GetPosition(),
        tanksPlayer0[2]->GetPosition(),
        tanksPlayer0[3]->GetPosition(),
        tanksPlayer1[0]->GetPosition(),
        tanksPlayer1[1]->GetPosition(),
        tanksPlayer1[2]->GetPosition(),
        tanksPlayer1[3]->GetPosition()
    };

    // conectar todos los tanques entre sí
    for(int i = 0; i < 8; i++){
        for(int j = i+1; j < 8; j++){
            EliminateObstacles(Pathfinding::SelectAlgorithm(
                TypePath::ASTAR_MAP, map, destinies[i], destinies[j]
            ));
        }
    }
}

#pragma endregion

#pragma region Helpers

bool Map::IsFLoor(const Position &pos){
    if (map[pos.i][pos.j]->GetType() != TypeElement::Floor)
        return false;
    return true;
}

bool Map::IsObstacle(const Position &pos){
    if (map[pos.i][pos.j]->GetType() != TypeElement::Obstacle)
        return false;
    return true;
}

bool Map::IsTank(const Position &pos){
    if (map[pos.i][pos.j]->GetType() != TypeElement::Tank)
        return false;
    return true;
}

bool Map::IsNullPlace(const Position &pos){
    if (map[pos.i][pos.j] == nullptr)
        return true;
    return false;
}

bool Map::IsPositionOcupied(const Position &pos){
    return !IsFLoor(pos) && !IsNullPlace(pos);
}

bool Map::IsRealPosition(const Position &pos){
    return (pos.i >= 0 && pos.i < GRID_ROWS && pos.j >= 0 && pos.j < GRID_COLS);
}

bool Map::IsRealPlayer(int id) { return (id == 0 || id == 1); }

void Map::EliminateObstacle(const Position &pos){
    if (!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if (!IsObstacle(pos)){
        throw std::invalid_argument("La posicion no corresponde a un obstaculo");
    }

    delete map[pos.i][pos.j];
    map[pos.i][pos.j] = new Floor(false);
}

void Map::EliminateTank(Tank *tank, const Position &pos){
    if (!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if (!IsTank(pos)){
        throw std::invalid_argument("La posicion no corresponde a un tanque");
    }
    if (tank->IsAlive()){
        throw std::invalid_argument("No se puede eliminar un tanque vivo");
    }

    int player = -1;
    int idx = -1;

    for (int i = 0; i < 4; i++){
        if (tanksPlayer0[i] == tank){
            player = 0;
            idx = i;
        }
        else if (tanksPlayer1[i] == tank){
            player = 1;
            idx = i;
        }
    }

    if (player == -1 || idx == -1){
        throw std::invalid_argument("El tanque no existe en el mapa ni en la lista de jugadores");
    }

    if (player == 0)
        tanksPlayer0[idx] = nullptr;
    else
        tanksPlayer1[idx] = nullptr;

    delete map[pos.i][pos.j];
    map[pos.i][pos.j] = new Floor(true);
}

void Map::MoveTank(Tank *tank, Position &destination){
    if (!IsRealPosition(destination)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if (!tank->IsAlive()){
        throw std::invalid_argument("No se puede eliminar un tanque vivo");
    }

    if (IsPositionOcupied(destination)){
        Position t = tank->GetPosition();
        Position newDestiny;
        int cost = INF;
        for (auto direction : allDirections){
            Position tmp = destination + direction;
            if (!IsRealPosition(tmp) || IsPositionOcupied(tmp))
                continue;

            int dx = abs(t.i - tmp.i);
            int dy = abs(t.j - tmp.j);
            if ((dx + dy) < cost){
                cost = dx + dy;
                newDestiny = tmp;
            }
        }

        if (cost == INF){
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

Element *Map::GetElementAt(const Position &pos){
    if (!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    return map[pos.i][pos.j];
}

Tank *Map::GetTankIn(const Position &pos){
    if (!IsRealPosition(pos)){
        throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
    }
    if (!IsTank(pos)){
        return nullptr;
    }
    return static_cast<Tank *>(map[pos.i][pos.j]);
}

Tank **Map::GetTanksOfPlayer(int idPlayer){
    if (!IsRealPlayer(idPlayer)){
        throw std::invalid_argument("El id del jugador no existe");
    }

    if (idPlayer == 0)
        return tanksPlayer0;

    return tanksPlayer1;
}

void Map::GetMap(Element *(&outMap)[GRID_ROWS][GRID_COLS]){
    for (int i = 0; i < GRID_ROWS; i++)
        for (int j = 0; j < GRID_COLS; j++)
            outMap[i][j] = map[i][j];
}

#pragma endregion
#include "include/Constants.h"
#include "Element.h"
#include "StructPosition.h"
#include "EnumColorTank.h"
#include "Tank.h"
#include "Obstacle_and_Floor.h"
#include <map>
#include <vector>
#include <stdexcept>
#include <random>
#ifndef MAP_H
#define MAP_H

class Map{
	private:
	Element* map[GRID_ROWS][GRID_COLS] = {nullptr};
	Tank** tanksPlayer0;
	Tank** tanksPlayer1;

	std::random_device rng{};
	std::mt19937 gen{rng()};
	std::uniform_int_distribution<int> binary{0, 1};
	std::uniform_int_distribution<int> disRows{0, GRID_ROWS};
	std::uniform_int_distribution<int> disCols{0, GRID_COLS};

	Map();

	// Rellena el mapa
	void  FillMap(); 

	// Se crean los tanques del jugador 1 y se ponen en una posicion aleatoria del mapa no ocupada
	void SetTankOfPlayer1(); 

	// Se crean los tanques del jugador 2 y se ponen en una posicion aleatoria del mapa no ocupada
	void SetTankOfPlayer2(); 

	// Recorre el mapa para verificar que cada Tank pueda llegar a los demas
	bool VerifyConectivity(const Position destinies[4]); 

	// Elimina obstaculos del mapa en base a una lista de posiciones
	void EliminateObstacles(const std::vector<Position> &pos);


	// Si no hay conectividad entonces forzamos que haya
	void CreateConectivity(); 

	// Verifica que el valor en la posicion (row, col) sea un suelo
	bool IsFLoor(const Position &pos);

	// Verifica que el valor en la posicion (row, col) sea un obstaculo
	bool IsObstacle(const Position &pos);

	// Verifica si el lugar es nulo
	bool IsNullPlace(const Position &pos);

	// Verifica si el lugar esta ocupado
	bool IsPositionOcupied(const Position &pos);

	//Verifica que el valor en la posicion (row, col) sea un tanque
	bool IsTank(const Position &pos);
	
	// Verifica si la posicion es real o posible
	bool IsRealPosition(const Position &pos);

	// Verifica si el id pertenece a un jugador
	bool IsRealPlayer(int id);

	public:

	// Asegurar existencia de un unico mapa en todo el juego
	static Map *GetInstance(){
		static Map instance;
		return &instance;
	}

	Map(const Map&) = delete;
	void operator=(const Map&) = delete;

	~Map();

	// Elimina el obstaculo por un suelo sin tumba
	void EliminateObstacle(const Position &pos);

	// Elimina el tanque y lo cambia por un floor con tumba
	void EliminateTank(Tank* tank, const Position &pos);

	void MoveTank(Tank* tank, Position &destination); // Intenta mover el tanque a la posicion (row, col) si no se puede manda un error

	// Devuelve el valor en la posicion (row, col)
	Element* GetElementAt(const Position &pos);

	// Devuelve el tanque en (row, col)
	Tank* GetTankIn(const Position &pos); 

	// Busca al tanque de ese color
	Tank* GetTankOfPlayer(int idPlayer, ColorTank color);
	
	// Devuelve la lista con los tanques pertenecientes al jugador
	Tank** GetTanksOfPlayer(int idPlayer);
};
#endif

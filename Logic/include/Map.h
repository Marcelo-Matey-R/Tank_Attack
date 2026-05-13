#include "include/Constants.h"
#include "Element.h"
#include "StructPosition.h"
#include "EnumColorTank.h"
#include "Tank.h"
#include <map>
#include <vector>
#ifndef MAP_H
#define MAP_H

class Map{
	private:
	Element* map[GRID_ROWS][GRID_COLS] = {nullptr};
	Tank** tanksPlayer1 = new Tank*[2];
	Tank** tanksPLayer2 = new Tank*[2];
	Map();
	static Map* instance;

	void  FillMap(); //Rellena el mapa

	void SetTankOfPlayer1(); //Se crean los tanques del jugador 1 y se ponen en una posicion aleatoria del mapa no ocupada
	void SetTankOfPlayer2(); //Se crean los tanques del jugador 2 y se ponen en una posicion aleatoria del mapa no ocupada

	bool VerifyConectivity(); //Recorre el mapa para verificar que cada Tank pueda llegar a los demas

	void CreateConectivity(); //Si no hay conectividad entonces forzamos que haya

	bool IsFLoor(const Position &pos); //Verifica que el valor en la posicion (row, col) sea un suelo
	bool IsObstacle(const Position &pos); //Verifica que el valor en la posicion (row, col) sea un obstaculo
	bool IsTank(const Position &pos); //Verifica que el valor en la posicion (row, col) sea un tanque

	public:
	/*
		Asegurar existencia de un unico mapa en todo el juego
	*/
	static Map *GetInstance();
	Map(const Map&) = delete;
	void operator=(const Map&) = delete;

	~Map();

	void EliminateObstacle(const Position &pos); // Elimina el obstaculo por un suelo sin tumba

	void EliminateTank(const Position &pos); // Elimina el tanque y lo cambia por un floor con tumba

	void MoveTank(Tank* tank, const Position &destination); // Intenta mover el tanque a la posicion (row, col) si no se puede manda un error

	int GetElementAt(const Position &pos); // Devuelve el valor en la posicion (row, col)

	Tank* GetTankIn(const Position &pos); // Devuelve el tanque en (row, col)

	Tank* GetTankOfPlayer(int idPlayer, ColorTank color); // Busca al tanque de ese color
	
	Tank** GetTanksOfPlayer(int idPlayer); // Devuelve la lista con los tanques pertenecientes al jugador
};
#endif

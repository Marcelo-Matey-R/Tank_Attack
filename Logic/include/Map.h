#include "include/Constants.h"
#include <map>
#include <vector>
#ifndef MAP_H
#define MAP_H
class Tank;
class Map{
	private:
	int map[MAP_HEIGHT][MAP_WIDTH];
	std::map<std::pair<int, int>, Tank*> posToTank;
	std::map<int, std::pair<int, int>> idTankToPos;
	Map();
	static Map* instance;

	void  FillMap(); //Rellena el mapa

	std::vector<int> CreateRows(); //Crea las filas con numeros que van del 0-5, los 4 tipos de Tank, piso y obstaculos

	bool VerifyConectivity(); //Recorre el mapa para verificar que cada Tank pueda llegar a los demas

	void CreateConectivity(); //Si no hay conectividad entonces forzamos que haya

	bool IsFLoor(int row, int col); //Verifica que el valor en la posicion (row, col) sea igual a la del piso

	public:
	static Map *GetInstance();
	Map(const Map&) = delete;
	void operator=(const Map&) = delete;

	~Map();
	void EliminateObstacle(int row, int col); //Cambia el valor en la posicion (row, col) al vlaor de piso

	void EliminateTank(int row, int col); //Cambia el valor de la posicion al del piso y elimna el valor de los unordered_map

	void MoveTank(Tank* tank, int row, int col); //Intenta mover el tanque a la posicion (row, col) si no se puede manda un error

	int GetValue(int row, int col); //Devuelve el valor en la posicion (row, col)

	Tank* GetTankIn(int row, int col); //Devuelve el tanque en (row, col)

	void SetTank(Tank* tank); //Busca la posicion en base al id del tanque e introduce las cosas al posToTank
};
#endif

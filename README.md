# Tank_Attack
Este proyecto consiste en el diseño e implementacion de un videojuego viejo llamado Tank Attack, intentando principalmente el uso de los algoritmos de pathfinding
en el mapa que es un grid 20x20, impidiendo los movimuientos diagonales.

## Requisitos

Para compilar el proyecto se necesita:

- CMake 3.25 o superior
- Un compilador C++ compatible con CMake
- En este proyecto se uso MSYS2 con `g++` y `mingw32-make`
- SFML a partir de 2.6.0 pero menores a la generacion 3.X

## Estructura del proyecto

- `include/`: carpeta para los headers de estructura de datos y Game.h
- `assets/`: carpeta para los fonts
- `Logic`: carpeta donde se encapsula la logica
- `UI`: carpeta donde se encapsula la parte visual
- `build/`: carpeta generada por CMake

## Ejecucion



## Compilación

Desde la raíz del proyecto:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/TankAttack
```

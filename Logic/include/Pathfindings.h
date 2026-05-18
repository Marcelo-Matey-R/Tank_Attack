#include <vector>
#include <map>
#include "StructPosition.h"
#include "Element.h"
#include "EnumTypePath.h"
#include "include/Constants.h"

#ifndef PATHFINDING_H
#define PATHFINDING_H

class Pathfinding{
    private:
    static std::vector<Position> ReconstructorPath(Position (*predecessors)[GRID_COLS], const Position &start, const Position &destiny);
    static int Heuristic(const Position &start, const Position &destiny);
    static std::vector<Position> Bfs(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static std::vector<Position> Dijkstra(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static bool IsInvalidPosition(const Position &neighborPos);
    static void FloodFill(Element* (&map)[GRID_ROWS][GRID_COLS], int (&zones)[GRID_ROWS][GRID_COLS], const Position &start, int actualColor);
    static std::vector<Position> A_Star_Game(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static void RandomMoves(Element* (&map)[GRID_ROWS][GRID_COLS], std::vector<Position> &pos, Position &start);
    static std::vector<Position> A_Star_Map(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static std::vector<Position> LOS(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    public:
    static bool IsConected(Element* (&map)[GRID_ROWS][GRID_COLS], const Position destinies[4]);
    static std::vector<Position> SelectAlgorithm(TypePath path, Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
};

#endif
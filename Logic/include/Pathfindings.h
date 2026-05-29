#include "../../include/MyArray.h"
#include <map>
#include "StructPosition.h"
#include "Element.h"
#include "EnumTypePath.h"
#include "../../UI/include/Constants.h"

#ifndef PATHFINDING_H
#define PATHFINDING_H

class Pathfinding{
    private:
    static Array<Position> ReconstructorPath(Position (*predecessors)[GRID_COLS], const Position &start, const Position &destiny);
    static int Heuristic(const Position &start, const Position &destiny);
    static Array<Position> Bfs(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static Array<Position> Dijkstra(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static bool IsInvalidPosition(const Position &neighborPos);
    static void FloodFill(Element* (&map)[GRID_ROWS][GRID_COLS], int (&zones)[GRID_ROWS][GRID_COLS], const Position &start, int actualColor);
    static Array<Position> A_Star_Game(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static void RandomMoves(Element* (&map)[GRID_ROWS][GRID_COLS], Array<Position> &pos, const Position &start);
    static Array<Position> A_Star_Map(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static bool MoveToThisDirections(Element *(&map)[GRID_ROWS][GRID_COLS], Array<Position>& positions, Position &start, const int &distance, const Position &directionToMove);
    static Array<Position> LOS(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    static Array<Position> AleatoryMovement(Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
    public:
    static bool IsConected(Element* (&map)[GRID_ROWS][GRID_COLS], const Position destinies[8]);
    static Array<Position> SelectAlgorithm(TypePath path, Element* (&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny);
};

#endif
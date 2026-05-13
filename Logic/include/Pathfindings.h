#include <vector>
#include "StructPosition.h"
#include "Element.h"
#include "EnumTypePath.h"

#ifndef PATHFINDING_H
#define PATHFINDING_H

class Pathfinding{
    private:
    static std::vector<Position> Bfs(Element* map, const Position &start, const Position &destiny);
    static std::vector<Position> Dijkstra(Element* map, const Position &start, const Position &destiny);
    static std::vector<Position> A_Star_Game(Element* map, const Position &start, const Position &destiny);
    static std::vector<Position> A_Star_Map(Element* map, const Position &start, const Position &destiny);
    static std::vector<Position> LOS(Element* map, const Position &start, const Position &destiny);
    public:
    static std::vector<Position> SelectAlgorithm(TypePath path);
};

#endif
#include "StructPosition.h"
#include "Element.h"

struct Node
{
    Position pos;
    int weight;

    bool operator<(const Node &n){
        return weight > n.weight;
    }
    bool operator>(const Node &n){
        return weight < n.weight;
    }
};

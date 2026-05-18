#ifndef STRUCTPOSITION_H
#define STRUCTPOSITION_H

struct Position{
    int i;
    int j;

    bool operator==(const Position& p) const{
        return (i == p.i && j == p.j);
    }
    bool operator!=(const Position& p) const{
        return (i != p.i && j != p.j);
    }
    bool operator<(const Position& p) const{
        return (i == p.i) ? j < p.j : p.i < i;
    }
    Position operator+(const Position& p) const{
        return Position{i + p.i, j + p.j};
    }
    Position operator+=(const Position &p){
        i += p.i;
        j += p.j;
        return *this;
    }
    Position operator-(const Position& p) const{
        return Position{abs(i - p.i), abs(j - p.j)};
    }
};

#endif
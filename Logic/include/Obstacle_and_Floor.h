#include <Element.h>

#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle : public Element{
    public:
    Obstacle() : Element(TypeElement::Obstacle){}
};

class Floor : public Element{
    private:
    bool isGrave;
    public:
    Floor(bool isGrave) : Element(TypeElement::Floor){
        this->isGrave = isGrave;
    }
    bool GetIsGrave(){return isGrave;}
};

#endif
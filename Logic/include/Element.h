#include "ETypeElement.h"
#ifndef ELEMENT_H
#define ELEMENT_H
#define INF 1e9
class Element{
    private:
    TypeElement type;
    int weightGame, weightMap;
    void SetWeight(TypeElement t){
        switch (t)
        {
        case TypeElement::Floor:
            weightMap = 1;
            weightGame = 1;
        case TypeElement::Obstacle:
            weightGame = INF;
            weightMap = 100;
        case TypeElement::Tank:
            weightGame = INF;
            weightMap = INF;
        }
    }
    public:
    Element(TypeElement type){
        this->type = type;
        SetWeight(type);
    }
    virtual ~Element() = default;
    TypeElement GetType(){return this->type;}
    int GetWeightGame(){return this->weightGame;}
    int GetWeightMap(){return this->weightMap;}
};

#endif
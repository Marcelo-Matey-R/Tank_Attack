#include <stdexcept>
#include <vector>
#include "Element.h"
#include "StructPosition.h"
#include "EnumColorTank.h"
#include "EnumPowerUp.h"
#include "EnumTypePath.h"
#include "include/Constants.h"

#ifndef TANK_H
#define TANK_H
class Bullet;
class Tank : public Element{
    private:
    int life;
    ColorTank color; // Valor que se utiliza el mapa
    Position position;
    virtual TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) = 0; // Da un numero para saber cual pathfinding se eligio (depende del tipo de tanque)
    public:
    Tank(ColorTank color) : Element(TypeElement::Tank){
        this->color = color;
        this->life = 100;
        Position pos = {0, 0};
        SetPosition(pos);
    }
    virtual ~Tank() = default;

    void SetPosition(const Position &n){
        if(n.i < 0 || n.j < 0 || n.i >= GRID_COLS || n.j >= GRID_ROWS) throw std::invalid_argument("La posicion no puede ser negativa o sobrepasar los limites del mapa");
        position.i = n.i; position.j = n.j;
    }

    virtual void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) = 0; // Cada tipo de tanque tiene en recibir danio
    virtual Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) = 0; // Cada tanque ataca a su manera    
    bool IsAlive(){return life > 0;} // Dice si el tanque esta vivo o no
    int GetLife(){return life;}
    ColorTank GetColor(){return color;} // Devuelve el color del tanque
    Position GetPosition(){return position;} // Devuelve la posicion
    std::vector<Position> GetPath(PowerUp movePrecise = PowerUp::NULL_POWERUP, Position destination); // Devuelve el Path para ir al destino

};

// Subclases de tanque
class TankLightBlueAndBlue : public Tank{
    private:
    TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) override;
    public:
    TankLightBlueAndBlue(ColorTank color) : Tank(color){}
    void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) override;
    Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) override;
};

class TankYellowAndRed : public Tank{
    private:
    TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) override;
    public:
    TankYellowAndRed(ColorTank color) : Tank(color){}
    void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) override;
    Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) override;
};

#endif
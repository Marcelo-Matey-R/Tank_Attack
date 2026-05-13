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
    bool isMovePrecise;

    public:
    Tank(ColorTank color) : Element(TypeElement::Tank){
        this->color = color;
        this->life = 100;
        this->isMovePrecise = false;
        Position pos = {0, 0};
        SetPosition(pos);
    }
    virtual ~Tank() = default;

    bool SetPosition(const Position &n){
        if(n.x < 0 || n.y < 0 || n.x >= GRID_COLS || n.y >= GRID_ROWS) return false;
        position.x = n.x; position.y = n.y;
        return true;
    }
    void SetMovePrecise(bool isPrecise){
        this->isMovePrecise = isPrecise;
    }
    virtual TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) = 0; // Da un numero para saber cual pathfinding se eligio (depende del tipo de tanque)
    virtual void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) = 0; // Cada tipo de tanque tiene en recibir danio
    virtual Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) = 0; // Cada tanque ataca a su manera    
    bool IsAlive(){return life <= 0;} // Dice si el tanque esta vivo o no
    ColorTank GetColor(){return color;} // Devuelve el color del tanque
    Position GetPosition(){return position;} // Devuelve la posicion
    std::vector<Position> GetPath(Position destination); // Devuelve el Path para ir al destino

};

// Subclases de tanque
class TankLightBlueAndBlue : public Tank{
    public:
    TankLightBlueAndBlue(ColorTank color) : Tank(color){}
    TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) override;
    void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) override;
    Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) override;
};

class TankYellowAndRed : public Tank{
    public:
    TankYellowAndRed(ColorTank color) : Tank(color){}
    TypePath SelectPathfinding(PowerUp movePrecise = PowerUp::NULL_POWERUP) override;
    void DecreaseHealth(PowerUp attackPlus = PowerUp::NULL_POWERUP) override;
    Bullet* Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny) override;
};

#endif
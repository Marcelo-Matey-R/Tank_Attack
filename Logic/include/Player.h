#include <queue>
#include "EnumPowerUp.h"
#include "Tank.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player{
    private:
    int id_player;
    std::queue<PowerUp> powerUps;
    PowerUp powerUpInUse;
    int turns;
    public:
    Player(int id){
        this->id_player = id;
        this->powerUpInUse = PowerUp::NULL_POWERUP;
    }
    void SetTurns();
    void GivePower(PowerUp up);
};

#endif
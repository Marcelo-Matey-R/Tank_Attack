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
        this->turns=1;
    }
    void SetTurns();
    void GivePower(PowerUp up);
    int GetId() const;
    PowerUp GetPowerUpInUse() const;
    int GetTurns() const;
    bool HasPowerUp() const;
    
    // consumir el power-up de la cola
    PowerUp ConsumePowerUp();
    
    // aplicar el power-up activo
    void ApplyPowerUp();
    
    // verificar si el turno del jugador terminó
    bool IsTurnOver() const;
    
    // resetear turno
    void ResetTurn();
    void UseTurn();
    
};

#endif
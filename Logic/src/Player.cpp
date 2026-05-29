#include "../include/Player.h"
#include "../include/EnumPowerUp.h"

#include <iostream>

void Player::SetTurns()
{
    turns = 1;
}

void Player::GivePower(PowerUp up)
{
    powerUps.Enqueue(up);
}

int Player::GetId() const
{
    return this->id_player;
}

PowerUp Player::GetPowerUpInUse() const
{
    return this->powerUpInUse;
}

int Player::GetTurns() const
{
    return this->turns;
}

bool Player::HasPowerUp() const
{
    return !powerUps.IsEmpty();
}

PowerUp Player::ConsumePowerUp()
{
    PowerUp powerToConsume = powerUps.Peek();
    powerUps.Dequeue();
    return powerToConsume;
}

void Player::ApplyPowerUp()
{
    if (powerUps.IsEmpty())
        return;
    powerUpInUse = ConsumePowerUp();
    debugger(powerUpInUse);
    // doble turno da 2 turnos en vez de 1
    if (powerUpInUse == PowerUp::DOUBLETURN)
        turns += 2;
}

bool Player::IsTurnOver() const
{
    return turns <= 0;
}

void Player::ResetTurn()
{
    turns = 1;
}

void Player::UseTurn()
{
    if (turns > 0)
        turns--;
}

void Player::debugger(PowerUp power)
{
    if (power == PowerUp::NULL_POWERUP)
        std::cout << "puto nulo" << '\n';
    else if (power == PowerUp::DOUBLETURN)
        std::cout << "puto doble retrasado" << '\n';
    else if (power == PowerUp::MOVEMENTPRECISION)
        std::cout << "puto movimiento preciso" << '\n';
    else if (power == PowerUp::ATTACKPOWER)
        std::cout << "puto poder" << '\n';
    else if (power == PowerUp::ATTACKACCURACY)
        std::cout << "puta precision" << '\n';
}
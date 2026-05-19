#include "../include/Player.h"
#include "../include/EnumPowerUp.h"

void Player::SetTurns() {
    turns=1;
}

void Player::GivePower(PowerUp up)
{
    powerUps.push(up);
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
    return !powerUps.empty();
}

PowerUp Player::ConsumePowerUp()
{
    PowerUp powerToConsume = powerUps.front();
    powerUps.pop();
    return powerToConsume;
}

void Player::ApplyPowerUp() {
    if (powerUps.empty()) return;
    
    powerUpInUse = ConsumePowerUp();
    
    // doble turno da 2 turnos en vez de 1
    if (powerUpInUse == PowerUp::DOUBLETURN)
        turns = 2;
}

bool Player::IsTurnOver() const{
 return turns<=0;
}

void Player::ResetTurn() {
 turns=1;
}

void Player::UseTurn() {
    if (turns > 0) turns--;
}
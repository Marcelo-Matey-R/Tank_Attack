#include "Tank.h"
#include "Map.h"
#include "Pathfindings.h"
#include "../../UI/include/Constants.h"
#include "../include/Bullet.h"

#pragma region Tank Light Blue and Tank Blue

std::vector<Position> Tank::GetPath(Position destination, Element *(&map)[GRID_ROWS][GRID_COLS], PowerUp movePrecise)
{
    TypePath pathType = SelectPathfinding(movePrecise);

    return Pathfinding::SelectAlgorithm(pathType, map, this->position, destination);
}
TypePath TankLightBlueAndBlue::SelectPathfinding(PowerUp movePrecise) {
    // solo lanzar excepción si el power-up no es válido para movimiento
    if (movePrecise != PowerUp::MOVEMENTPRECISION && 
        movePrecise != PowerUp::NULL_POWERUP) {
        movePrecise = PowerUp::NULL_POWERUP;  // ← ignorar en vez de lanzar excepción
    }

    int prob = dis(gen);
    
    if (movePrecise == PowerUp::NULL_POWERUP) {
        if (prob <= 5) return TypePath::BFS;
        else return TypePath::ALEATORYMOVEMENT;
    }
    
    // con power-up de precisión — 90% BFS
    if (prob <= 9) return TypePath::BFS;
    else return TypePath::ALEATORYMOVEMENT;
}

void TankLightBlueAndBlue::DecreaseHealth(PowerUp attackPlus) {
    // ignorar power-ups inválidos en vez de lanzar excepción
    if (attackPlus != PowerUp::ATTACKPOWER && attackPlus != PowerUp::NULL_POWERUP) {
        attackPlus = PowerUp::NULL_POWERUP;
    }

    if (attackPlus == PowerUp::NULL_POWERUP) {
        life -= 25.0f;
        if (life < 0) life = 0;
    }
    else life = 0;
}

Bullet *TankLightBlueAndBlue::Attack(const Position &destiny, PowerUp attackPlus, PowerUp attackAccuracy){
    // daño base 25% para azul/celeste
    float damage = 25.0f;

    // con power-up de ataque el daño es 100%
    if (attackPlus == PowerUp::ATTACKPOWER)
        damage = 100.0f;

    return new Bullet(this->position, destiny, damage, attackAccuracy);
}

#pragma endregion

#pragma region Tank Yellow and Tank Red


TypePath TankYellowAndRed::SelectPathfinding(PowerUp movePrecise) {
    if (movePrecise != PowerUp::MOVEMENTPRECISION && movePrecise != PowerUp::NULL_POWERUP) {
        movePrecise = PowerUp::NULL_POWERUP;  // ← ignorar en vez de lanzar excepción
    }

    int prob = dis(gen);
    
    if (movePrecise == PowerUp::NULL_POWERUP) {
        if (prob <= 8) return TypePath::DIJKSTRA;  // 80% Dijkstra
        else return TypePath::ALEATORYMOVEMENT;
    }
    
    // con power-up de precisión — 90% Dijkstra
    if (prob <= 9) return TypePath::DIJKSTRA;
    else return TypePath::ALEATORYMOVEMENT;
}

void TankYellowAndRed::DecreaseHealth(PowerUp attackPlus) {
    if (attackPlus != PowerUp::ATTACKPOWER && 
        attackPlus != PowerUp::NULL_POWERUP) {
        attackPlus = PowerUp::NULL_POWERUP;
    }

    if (attackPlus == PowerUp::NULL_POWERUP) {
        life -= 50.0f;
        if (life < 0) life = 0;
    }
    else life = 0;
}
Bullet *TankYellowAndRed::Attack(const Position &destiny, PowerUp attackPlus, PowerUp attackAccuracy){
    // daño base 50% para rojo/amarillo
    float damage = 50.0f;

    // con power-up de ataque el daño es 100%
    if (attackPlus == PowerUp::ATTACKPOWER)
        damage = 100.0f;

    return new Bullet(this->position, destiny, damage, attackAccuracy);
}

#pragma endregion
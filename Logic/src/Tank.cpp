#include "Tank.h"

#pragma region Tank Light Blue and Tank Blue

TypePath TankLightBlueAndBlue::SelectPathfinding(PowerUp movePrecise){
    if(movePrecise != PowerUp::MOVEMENTPRECISION || movePrecise != PowerUp::NULL_POWERUP){
        throw std::invalid_argument("El argumento del power up solo puede ser el de movimiento precisor o aleatorio");
    }

    int prob = dis(gen);
    if(movePrecise == PowerUp::NULL_POWERUP){
        if(prob < 5) return TypePath::BFS;
        else return TypePath::ALEATORYMOVEMENT;
    }
    
    if(prob <= 10){
        return TypePath::BFS;
    }
    else TypePath::ALEATORYMOVEMENT;
}

void TankLightBlueAndBlue::DecreaseHealth(PowerUp attackPlus){
    if(attackPlus != PowerUp::ATTACKPOWER || attackPlus != PowerUp::NULL_POWERUP){
        throw std::invalid_argument("El argumento del power up solo puede ser el de movimiento precisor o aleatorio");
    }

    if(attackPlus == PowerUp::NULL_POWERUP){
        life -= life/4;
    }
    else life = 0;
}

Bullet* TankLightBlueAndBlue::Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny){

}

#pragma endregion


#pragma region Tank Yellow and Tank Red

TypePath TankYellowAndRed::SelectPathfinding(PowerUp movePrecise){
    if(movePrecise != PowerUp::MOVEMENTPRECISION || movePrecise != PowerUp::NULL_POWERUP){
        throw std::invalid_argument("El argumento del power up solo puede ser el de movimiento precisor o aleatorio");
    }

    int prob = dis(gen);
    if(movePrecise == PowerUp::NULL_POWERUP){
        if(prob < 5) return TypePath::BFS;
        else return TypePath::ALEATORYMOVEMENT;
    }
    
    if(prob <= 10){
        return TypePath::BFS;
    }
    else TypePath::ALEATORYMOVEMENT;
}

void TankYellowAndRed::DecreaseHealth(PowerUp attackPlus){
    if(attackPlus != PowerUp::ATTACKPOWER || attackPlus != PowerUp::NULL_POWERUP){
        throw std::invalid_argument("El argumento del power up solo puede ser el de movimiento precisor o aleatorio");
    }

    if(attackPlus == PowerUp::NULL_POWERUP){
        life -= life/2;
    }
    else life = 0;
}

Bullet* TankYellowAndRed::Attack(PowerUp attackPlus = PowerUp::NULL_POWERUP, PowerUp attackAccuracy = PowerUp::NULL_POWERUP, const Position &destiny){

}
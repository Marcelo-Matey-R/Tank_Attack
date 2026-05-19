#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "StructPosition.h"
#include "EnumPowerUp.h"
#include "../../UI/include/Constants.h"
#include <random>

class Map;

class Bullet {
public:
    Bullet(Position origin, Position destination,
           float damage, PowerUp attackAccuracy = PowerUp::NULL_POWERUP);

    // calcula la trayectoria completa con rebotes
    void CalculatePath(Map* map);

    // avanza la bala un paso
    void Step();

    // getters para Renderer
    std::vector<Position> GetPath() const;
    sf::Vector2f GetCurrentPixelPosition() const;
    Position GetCurrentCell() const;
    float GetDamage() const;
    bool IsActive() const;
    bool HasReachedDestination() const;
    int GetMaxBounces() const;
    void Deactivate();
private:
    std::random_device rng;
    std::mt19937 gen;
    // origen y destino
    Position origin;
    Position destination;

    // posición actual en píxeles (para movimiento suave)
    float x, y;

    // vector de dirección normalizado * velocidad
    float dx, dy;

    // daño que aplica
    float damage;

    // estado
    bool active;
    int currentStep;
    int bounces;
    int maxBounces;

    // trayectoria precalculada en celdas
    std::vector<Position> path;

    // power-up aplicado
    PowerUp attackAccuracy;

    // helpers de colisión
    bool IsWall(Map* map, Position cell) const;
    bool IsOutOfBounds(Position cell) const;
    bool IsTank(Map* map, Position cell) const;

    // calcula rebote según tipo de pared tocada
    void ApplyRebound(Map* map, Position cell);

    // convierte posición en píxeles a celda
    Position PixelToCell() const;

    // inicializa dx y dy desde origen hacia destino
    void InitDirection();
};
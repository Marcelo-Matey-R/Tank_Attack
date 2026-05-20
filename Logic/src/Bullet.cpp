#include "../include/Bullet.h"
#include "../include/Map.h"
#include "../include/Directions.h"
#include "../include/StructPosition.h"
#include "../../UI/include/Constants.h"
#include <cmath>
#include <random>

Bullet::Bullet(Position origin, Position destination,
               float damage, PowerUp attackAccuracy)
    : origin(origin), destination(destination),
      damage(damage), attackAccuracy(attackAccuracy), rng(), gen(rng())
{
    // posición inicial en píxeles (centro de la celda)
    x = origin.j * CELL_SIZE + CELL_SIZE / 2.0f;
    y = origin.i * CELL_SIZE + CELL_SIZE / 2.0f;

    // estado inicial
    active = true;
    currentStep = 0;
    bounces = 0;

    // número aleatorio de rebotes
    std::uniform_int_distribution<int> disBounces(BULLET_MIN_BOUNCES, BULLET_MAX_BOUNCES);
    maxBounces = disBounces(gen);

    // inicializar dirección
    InitDirection();
}

void Bullet::CalculatePath(Map* map) {
    path.clear();
    
    // posición temporal para simular
    float simX = x;
    float simY = y;
    float simDx = dx;
    float simDy = dy;
    int simBounces = 0;

    // límite de pasos para evitar loop infinito
    int maxSteps = GRID_ROWS * GRID_COLS * 4;

    for (int step = 0; step < maxSteps; step++) {
        // avanzar posición
        simX += simDx;
        simY += simDy;

        // convertir a celda
        Position cell = {
            (int)(simY / CELL_SIZE),
            (int)(simX / CELL_SIZE)
        };

        // verificar si salió del mapa
        if (IsOutOfBounds(cell)) {
            break;
        }

        // agregar celda al path si es nueva
        if (path.empty() || path.back() != cell) {
            path.push_back(cell);
        }

        // verificar qué hay en la celda
        Element* elem = map->GetElementAt(cell);

        if (elem == nullptr) break;

        if (elem->GetType() == TypeElement::Obstacle) {
            if (simBounces >= maxBounces) break;  // agotó rebotes

            // calcular tipo de rebote
            // verificar celda horizontal (mismo i, diferente j)
            Position horizontalCell = {cell.i, (int)((simX - simDx) / CELL_SIZE)};
            Position verticalCell   = {(int)((simY - simDy) / CELL_SIZE), cell.j};

            bool hitVerticalWall   = IsWall(map, horizontalCell);
            bool hitHorizontalWall = IsWall(map, verticalCell);

            if (hitVerticalWall && hitHorizontalWall) {
                // esquina — invertir ambos
                simDx = -simDx;
                simDy = -simDy;
            } else if (hitVerticalWall) {
                simDx = -simDx;
            } else {
                simDy = -simDy;
            }

            simBounces++;

            // retroceder para no quedar dentro del obstáculo
            simX -= simDx;
            simY -= simDy;

        } else if (IsTank(map, cell)) {
            // llegó a un tanque — terminar trayectoria
            break;
        }

        // verificar si llegó al destino
        if (cell == destination) break;
    }
}

void Bullet::Step() {
    if (!active) return;

    // avanzar posición en píxeles
    x += dx;
    y += dy;

    // actualizar celda actual
    Position cell = PixelToCell();

    // verificar si salió del mapa
    if (IsOutOfBounds(cell)) {
        active = false;
        return;
    }

    // avanzar en el path precalculado
    if (currentStep < path.size() - 1)
        currentStep++;
    else
        active = false;  // llegó al final del path
}

std::vector<Position> Bullet::GetPath() const {
    return path;
}

sf::Vector2f Bullet::GetCurrentPixelPosition() const {
    return sf::Vector2f(x, y);
}

Position Bullet::GetCurrentCell() const {
    return PixelToCell();
}

float Bullet::GetDamage() const {
    return damage;
}

bool Bullet::IsActive() const {
    return active;
}

bool Bullet::HasReachedDestination() const {
    return !active || GetCurrentCell() == destination;
}

int Bullet::GetMaxBounces() const {
    return maxBounces;
}

void Bullet::Deactivate() {
    active = false;
}

void Bullet::InitDirection() {
    // destino en píxeles (centro de la celda)
    float destX = destination.j * CELL_SIZE + CELL_SIZE / 2.0f;
    float destY = destination.i * CELL_SIZE + CELL_SIZE / 2.0f;

    // vector hacia el destino
    float diffX = destX - x;
    float diffY = destY - y;

    // normalizar
    float length = std::sqrt(diffX * diffX + diffY * diffY);
    if (length > 0) {
        dx = (diffX / length) * BULLET_SPEED;
        dy = (diffY / length) * BULLET_SPEED;
    } else {
        dx = 0;
        dy = 0;
        active = false;  // origen == destino, bala inválida
    }
}

Position Bullet::PixelToCell() const {
    return {
        (int)(y / CELL_SIZE),
        (int)(x / CELL_SIZE)
    };
}

bool Bullet::IsOutOfBounds(Position cell) const {
    return cell.i < 0 || cell.i >= GRID_ROWS ||
           cell.j < 0 || cell.j >= GRID_COLS;
}

bool Bullet::IsWall(Map* map, Position cell) const {
    if (IsOutOfBounds(cell)) return true;
    Element* elem = map->GetElementAt(cell);
    return elem != nullptr && 
           elem->GetType() == TypeElement::Obstacle;
}

bool Bullet::IsTank(Map* map, Position cell) const {
   if (IsOutOfBounds(cell)) return false;
    
    Element* elem = map->GetElementAt(cell);
    if (elem == nullptr || elem->GetType() != TypeElement::Tank) return false;

    // Ignorar al tank de origen solo si no ha rebotado
    if (cell == origin && bounces == 0) return false;

    return true;
}
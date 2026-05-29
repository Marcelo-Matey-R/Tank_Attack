#include "../include/Bullet.h"
#include "../include/Map.h"
#include "../include/Directions.h"
#include "../include/StructPosition.h"
#include "../../UI/include/Constants.h"
#include "../include/Pathfindings.h"
#include "../include/EnumTypePath.h"
#include <cmath>
#include <random>
#include <iostream>

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

void Bullet::CalculatePath(Map *map)
{
    path.Clear();

    if (attackAccuracy == PowerUp::ATTACKACCURACY)
    {
        Element *mapGrid[GRID_ROWS][GRID_COLS];
        map->GetMap(mapGrid);
        path = Pathfinding::SelectAlgorithm(
            TypePath::ASTAR_GAME,
            mapGrid,
            origin,
            destination);
        return;
    }

    // trabajar en celdas en vez de pixeles
    float simRow = origin.i + 0.5f; // centro de la celda en filas
    float simCol = origin.j + 0.5f; // centro de la celda en columnas

    float length = std::sqrt(dx * dx + dy * dy);
    float simDrow = (dy / length) * 0.1f; // paso pequeño en filas
    float simDcol = (dx / length) * 0.1f; // paso pequeño en columnas

    int maxSteps = GRID_ROWS * GRID_COLS * 100;
    Position lastCell = origin;

    for (int step = 0; step < maxSteps; step++)
    {
        simRow += simDrow;
        simCol += simDcol;

        Position cell = {(int)simRow, (int)simCol};

        if (IsOutOfBounds(cell))
            break;

        // solo procesamos en el caso de que la bala haya entrado a una nueva celda
        if (cell == lastCell)
            continue;
        lastCell = cell;

        Element *elem = map->GetElementAt(cell);
        if (elem == nullptr)
            break;

        if (elem->GetType() == TypeElement::Obstacle)
        {
            if (bounces >= maxBounces)
                break;

            // retroceder a celda anterior
            simRow -= simDrow * 2;
            simCol -= simDcol * 2;

            // verificar tipo de rebote
            Position cellX = {(int)simRow, (int)(simCol + simDcol * 2)};
            Position cellY = {(int)(simRow + simDrow * 2), (int)simCol};

            bool wallOnX = !IsOutOfBounds(cellX) && IsWall(map, cellX);
            bool wallOnY = !IsOutOfBounds(cellY) && IsWall(map, cellY);

            if (wallOnX && wallOnY)
            {
                simDcol = -simDcol;
                simDrow = -simDrow;
            }
            else if (wallOnX)
            {
                simDcol = -simDcol;
            }
            else if (wallOnY)
            {
                simDrow = -simDrow;
            }
            else
            {
                simDcol = -simDcol;
                simDrow = -simDrow;
            }

            bounces++;
            lastCell = {(int)simRow, (int)simCol};
        }
        else if (IsTank(map, cell))
        {
            path.PushBack(cell);
            break;
        }
        else
        {
            path.PushBack(cell);
        }
    }
}

void Bullet::Step()
{
    if (!active)
        return;

    // avanzar al siguiente paso del path
    if (currentStep < (int)path.Size() - 1)
    {
        currentStep++;
        // actualizar posición en píxeles al centro de la celda actual
        Position cell = path[currentStep];
        x = cell.j * CELL_SIZE + CELL_SIZE / 2.0f;
        y = cell.i * CELL_SIZE + CELL_SIZE / 2.0f;
    }
    else
    {
        active = false;
    }
}

Array<Position> Bullet::GetPath() const
{
    return path;
}

sf::Vector2f Bullet::GetCurrentPixelPosition() const
{
    return sf::Vector2f(x, y);
}

Position Bullet::GetCurrentCell() const
{
    return PixelToCell();
}

float Bullet::GetDamage() const
{
    return damage;
}

bool Bullet::IsActive() const
{
    return active;
}

bool Bullet::HasReachedDestination() const
{
    return !active || GetCurrentCell() == destination;
}

int Bullet::GetMaxBounces() const
{
    return maxBounces;
}

void Bullet::Deactivate()
{
    active = false;
}

void Bullet::InitDirection()
{
    float destX = destination.j * CELL_SIZE + CELL_SIZE / 2.0f;
    float destY = destination.i * CELL_SIZE + CELL_SIZE / 2.0f;
    float diffX = destX - x;
    float diffY = destY - y;
    float length = std::sqrt(diffX * diffX + diffY * diffY);
    if (length > 0)
    {
        dx = (diffX / length) * BULLET_SPEED;
        dy = (diffY / length) * BULLET_SPEED;
    }
    std::cout << "dx: " << dx << " dy: " << dy << std::endl;
}

Position Bullet::PixelToCell() const
{
    return {
        (int)(y / CELL_SIZE),
        (int)(x / CELL_SIZE)};
}

bool Bullet::IsOutOfBounds(Position cell) const
{
    return cell.i < 0 || cell.i >= GRID_ROWS ||
           cell.j < 0 || cell.j >= GRID_COLS;
}

bool Bullet::IsWall(Map *map, Position cell) const
{
    if (IsOutOfBounds(cell))
        return true;
    Element *elem = map->GetElementAt(cell);
    return elem != nullptr &&
           elem->GetType() == TypeElement::Obstacle;
}

bool Bullet::IsTank(Map *map, Position cell) const
{
    if (IsOutOfBounds(cell))
        return false;

    Element *elem = map->GetElementAt(cell);
    if (elem == nullptr || elem->GetType() != TypeElement::Tank)
        return false;

    // Ignorar al tank de origen solo si no ha rebotado
    if (cell == origin && bounces == 0)
        return false;

    return true;
}
#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

enum class ActionType {
    None,
    LeftClick,        
    RightClick,       
    ShiftPressed,     
    CloseWindow
};

struct PlayerAction {
    ActionType type;
    sf::Vector2i cellSelected;  // celda pero en coordenadas de la cuadricula
    int playerID;               // jugador que realizo la accion
};

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window);

    // procesa todos los eventos del frame actual
    // devuelve la acción que realizo el jugador
    PlayerAction processEvents();

    // devuelve la celda sobre la que esta el cursor actualmente
    sf::Vector2i getHoveredCell() const;

    // devuelve la posición del mouse en pixeles
    sf::Vector2i getMousePosition() const;

    // verifica si el mouse esta dentro del area del mapa
    bool isMouseOnMap() const;

    sf::Event getLastEvent() const;

private:
    sf::RenderWindow& window;
    sf::Vector2i hoveredCell;
    sf::Vector2i lastClickedCell;
    sf::Event lastEvent;
    // pasa de pixeles a cuadriculas
    sf::Vector2i pixelToCell(sf::Vector2i pixelPos) const;

};
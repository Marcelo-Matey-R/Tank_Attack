#include "../include/InputHandler.h"
#include "../include/Constants.h"

InputHandler::InputHandler(sf::RenderWindow& window)
    : window(window)
{
    hoveredCell = sf::Vector2i(-1, -1); 
    lastClickedCell = sf::Vector2i(-1, -1);
    lastEvent = sf::Event();
}

PlayerAction InputHandler::processEvents() {
    PlayerAction action;
    action.type = ActionType::None;
    action.cellSelected = sf::Vector2i(-1, -1);

    while (window.pollEvent(lastEvent)) {
        switch (lastEvent.type) {
            case sf::Event::Closed:
                action.type = ActionType::CloseWindow;
                break;

            case sf::Event::MouseButtonPressed:
                if (lastEvent.mouseButton.button == sf::Mouse::Left) {
                    action.type = ActionType::LeftClick;
                    action.cellSelected = pixelToCell(
                        sf::Vector2i(lastEvent.mouseButton.x, 
                                     lastEvent.mouseButton.y)
                    );
                }
                else if (lastEvent.mouseButton.button == sf::Mouse::Right) {
                    action.type = ActionType::RightClick;
                    action.cellSelected = pixelToCell(
                        sf::Vector2i(lastEvent.mouseButton.x,
                                     lastEvent.mouseButton.y)
                    );
                }
                break;

            case sf::Event::KeyPressed:
                if (lastEvent.key.code == sf::Keyboard::LShift ||
                    lastEvent.key.code == sf::Keyboard::RShift) {
                    action.type = ActionType::ShiftPressed;
                }
                break;

            case sf::Event::MouseMoved:
                hoveredCell = pixelToCell(
                    sf::Vector2i(lastEvent.mouseMove.x,
                                 lastEvent.mouseMove.y)
                );
                break;

            default:
                break;
        }
    }

    return action;
}

sf::Vector2i InputHandler::getHoveredCell() const {
    return hoveredCell;
}

sf::Vector2i InputHandler::getMousePosition() const {
    return sf::Mouse::getPosition(window);
}

bool InputHandler::isMouseOnMap() const {
    sf::Vector2i mousePos = getMousePosition();
    return mousePos.x >= 0 && mousePos.x < MAP_WIDTH &&
           mousePos.y >= 0 && mousePos.y < MAP_HEIGHT;
}

sf::Event InputHandler::getLastEvent() const{
    return lastEvent;
}

sf::Vector2i InputHandler::pixelToCell(sf::Vector2i pixelPos) const {
    return sf::Vector2i(
        pixelPos.x / CELL_SIZE,
        pixelPos.y / CELL_SIZE
    );
}
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class HUD {
public:
    HUD(sf::RenderWindow& window, sf::Font& font);

    // dibuja todo el HUD
    void draw();

    // actualizar información del HUD
    void updateTimer(int secondsRemaining);
    void updateCurrentPlayer(int playerID);
    void updateTankHealth(int playerID, int tankIndex, float healthPercent);
    void updatePowerUpQueue(int playerID, const std::vector<std::string>& powerUps);

private:
    sf::RenderWindow& window;
    sf::Font& font;

    // fondo del HUD
    sf::RectangleShape background;

    // timer
    sf::Text timerText;
    void drawTimer();

    // turno actual
    sf::Text turnText;
    void drawCurrentPlayer();

    // vida de tanques
    // [jugador][tanque] → 4 tanques por jugador
    float tankHealth[2][4];
    sf::RectangleShape healthBarBackground[2][4];
    sf::RectangleShape healthBar[2][4];
    sf::Text tankLabel[2][4];
    void drawTankHealth();

    // power-ups en cola
    std::vector<std::string> powerUpQueue[2];
    sf::Text powerUpText[2];
    void drawPowerUpQueue();

    // para colocar los textos
    sf::Text createText(const std::string& str, int fontSize,
                        sf::Color color, sf::Vector2f position);
};
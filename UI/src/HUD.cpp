#include "../include/HUD.h"
#include "../include/Constants.h"

HUD::HUD(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font)
{
    // fondo del HUD
    background.setSize(sf::Vector2f(HUD_WIDTH, WINDOW_HEIGHT));
    background.setPosition(MAP_WIDTH, 0.f);
    background.setFillColor(sf::Color(20, 20, 20));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(1.f);

    // inicializar vida de tanques
    for (int player = 0; player < 2; player++) {
        for (int tank = 0; tank < 4; tank++) {
            tankHealth[player][tank] = 1.0f;  // 100% de vida al inicio

            // fondo de la barra de vida
            healthBarBackground[player][tank].setSize(sf::Vector2f(150.f, 12.f));
            healthBarBackground[player][tank].setFillColor(sf::Color(60, 60, 60));

            // barra de vida
            healthBar[player][tank].setSize(sf::Vector2f(150.f, 12.f));
            healthBar[player][tank].setFillColor(sf::Color::Green);

            // etiqueta del tanque
            tankLabel[player][tank].setFont(font);
            tankLabel[player][tank].setCharacterSize(8);
            tankLabel[player][tank].setFillColor(sf::Color::White);
        }
    }

    // inicializar textos
    timerText.setFont(font);
    timerText.setCharacterSize(HUD_FONT_SIZE);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(MAP_WIDTH + HUD_PADDING, 20.f);

    turnText.setFont(font);
    turnText.setCharacterSize(HUD_FONT_SIZE);
    turnText.setFillColor(sf::Color::White);
    turnText.setPosition(MAP_WIDTH + HUD_PADDING, 60.f);
}

void HUD::draw() {
    // fondo del HUD
    window.draw(background);
    
    // contenido
    drawTimer();
    drawCurrentPlayer();
    drawTankHealth();
    drawPowerUpQueue();
}

void HUD::updateTimer(int secondsRemaining) {
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining % 60;
    
    // formato MM:SS
    std::string time = std::to_string(minutes) + ":" +
                       (seconds < 10 ? "0" : "") + std::to_string(seconds);
    timerText.setString("TIEMPO: " + time);
}

void HUD::updateCurrentPlayer(int playerID) {
    turnText.setString("TURNO: J" + std::to_string(playerID));
}

void HUD::updateTankHealth(int playerID, int tankIndex, float healthPercent) {
    tankHealth[playerID - 1][tankIndex] = healthPercent;
    
    // actualizar ancho de la barra según el porcentaje
    healthBar[playerID - 1][tankIndex].setSize(
        sf::Vector2f(150.f * healthPercent, 12.f)
    );
    
    // cambiar color según vida restante
    if (healthPercent > 0.5f)
        healthBar[playerID - 1][tankIndex].setFillColor(sf::Color::Green);
    else if (healthPercent > 0.25f)
        healthBar[playerID - 1][tankIndex].setFillColor(sf::Color::Yellow);
    else
        healthBar[playerID - 1][tankIndex].setFillColor(sf::Color::Red);
}

void HUD::updatePowerUpQueue(int playerID, const std::vector<std::string>& powerUps) {
    powerUpQueue[playerID - 1] = powerUps;
    
    std::string text = "J" + std::to_string(playerID) + ": ";
    if (powerUps.empty()) {
        text += "ninguno";
    } else {
        for (int i = 0; i < powerUps.size(); i++) {
            text += powerUps[i];
            if (i < powerUps.size() - 1)
                text += ", ";
        }
    }
    powerUpText[playerID - 1].setString(text);
}

void HUD::drawTimer() {
    window.draw(timerText);
}

void HUD::drawCurrentPlayer() {
    window.draw(turnText);
}

void HUD::drawTankHealth() {
    float startY = 120.f;      // posición Y donde empiezan las barras
    float spacingY = 35.f;     // espacio entre cada barra
    float barX = MAP_WIDTH + HUD_PADDING;

    // colores por jugador y tanque
    sf::Color colors[2][4] = {
        {sf::Color::Blue, sf::Color::Blue,
         sf::Color::Red,  sf::Color::Red},       // jugador 1
        {sf::Color::Cyan, sf::Color::Cyan,
         sf::Color::Yellow, sf::Color::Yellow}   // jugador 2
    };

    std::string labels[2][4] = {
        {"Azul 1", "Azul 2", "Rojo 1", "Rojo 2"},
        {"Celeste 1", "Celeste 2", "Amarillo 1", "Amarillo 2"}
    };

    for (int player = 0; player < 2; player++) {
        // título del jugador
        sf::Text playerTitle;
        playerTitle.setFont(font);
        playerTitle.setCharacterSize(HUD_FONT_SIZE);
        playerTitle.setFillColor(sf::Color::White);
        playerTitle.setString("JUGADOR " + std::to_string(player + 1));
        playerTitle.setPosition(barX, startY - 25.f + (player * 4 * spacingY + player * 30.f));
        window.draw(playerTitle);

        for (int tank = 0; tank < 4; tank++) {
            float y = startY + (tank * spacingY) + (player * 4 * spacingY + player * 30.f);

            // etiqueta del tanque
            tankLabel[player][tank].setString(labels[player][tank]);
            tankLabel[player][tank].setFillColor(colors[player][tank]);
            tankLabel[player][tank].setPosition(barX, y);
            window.draw(tankLabel[player][tank]);

            // fondo de la barra
            healthBarBackground[player][tank].setPosition(barX, y + 14.f);
            window.draw(healthBarBackground[player][tank]);

            // barra de vida
            healthBar[player][tank].setPosition(barX, y + 14.f);
            window.draw(healthBar[player][tank]);
        }
    }
}

void HUD::drawPowerUpQueue() {
    float startY = 650.f;  // posición Y debajo de las barras de vida
    float barX = MAP_WIDTH + HUD_PADDING;

    // título
    sf::Text powerTitle;
    powerTitle.setFont(font);
    powerTitle.setCharacterSize(HUD_FONT_SIZE);
    powerTitle.setFillColor(sf::Color::White);
    powerTitle.setString("POWER-UPS");
    powerTitle.setPosition(barX, startY);
    window.draw(powerTitle);

    // power-ups por jugador
    for (int player = 0; player < 2; player++) {
        powerUpText[player].setFont(font);
        powerUpText[player].setCharacterSize(8);
        powerUpText[player].setPosition(barX, startY + 30.f + (player * 40.f));
        window.draw(powerUpText[player]);
    }
}

sf::Text HUD::createText(const std::string& str, int fontSize,
                          sf::Color color, sf::Vector2f position) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}
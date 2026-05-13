#include "../include/Renderer.h"
#include "../include/Constants.h"
#include "../include/Button.h"

Renderer::Renderer(sf::RenderWindow &window, sf::Font &font) : window(window), font(font)
{
    useSprites = false;
}

void Renderer::drawMainMenu(const std::vector<Button*>& buttons) {
    // título
    sf::Text title;
    title.setFont(font);
    title.setString("TANK ATTACK!");
    title.setCharacterSize(MENU_TITLE_FONT_SIZE);
    title.setFillColor(sf::Color::Yellow);
    
    // centrar título horizontalmente
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f - titleBounds.left,
        150.f
    );
    
    window.draw(title);

    // botones
    for (auto& button : buttons)
        button->draw(window);
}

void Renderer::drawInstructions(const std::vector<Button*>& buttons) {
    // título
    sf::Text title;
    title.setFont(font);
    title.setString("INSTRUCCIONES");
    title.setCharacterSize(MENU_TITLE_FONT_SIZE);
    title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f - titleBounds.left,
        50.f
    );
    window.draw(title);

    // sección controles
    sf::Text controlsTitle;
    controlsTitle.setFont(font);
    controlsTitle.setString("CONTROLES");
    controlsTitle.setCharacterSize(HUD_TITLE_FONT_SIZE);
    controlsTitle.setFillColor(sf::Color::White);
    controlsTitle.setPosition(100.f, 150.f);
    window.draw(controlsTitle);

    // imagen de controles
    sf::RectangleShape controlsImg(sf::Vector2f(300.f, 200.f));
    controlsImg.setPosition(100.f, 190.f);
    controlsImg.setFillColor(sf::Color(50, 50, 50));
    controlsImg.setOutlineColor(sf::Color::White);
    controlsImg.setOutlineThickness(1.f);
    window.draw(controlsImg);

    // texto de controles
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString("Click izq: mover\nClick der: disparar\nShift: power-up");
    controlsText.setCharacterSize(HUD_FONT_SIZE);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setPosition(100.f, 410.f);
    window.draw(controlsText);

    // sección power-ups
    sf::Text powerTitle;
    powerTitle.setFont(font);
    powerTitle.setString("POWER-UPS");
    powerTitle.setCharacterSize(HUD_TITLE_FONT_SIZE);
    powerTitle.setFillColor(sf::Color::White);
    powerTitle.setPosition(580.f, 150.f);
    window.draw(powerTitle);

    // imagen power-ups
    sf::RectangleShape powerImg(sf::Vector2f(300.f, 200.f));
    powerImg.setPosition(580.f, 190.f);
    powerImg.setFillColor(sf::Color(50, 50, 50));
    powerImg.setOutlineColor(sf::Color::White);
    powerImg.setOutlineThickness(1.f);
    window.draw(powerImg);

    // texto power-ups
    sf::Text powerText;
    powerText.setFont(font);
    powerText.setString("Doble turno\nPrecision mov\nPrecision ataque\nPoder ataque");
    powerText.setCharacterSize(HUD_FONT_SIZE);
    powerText.setFillColor(sf::Color::White);
    powerText.setPosition(580.f, 410.f);
    window.draw(powerText);

    // botón volver
    for (auto& button : buttons)
        button->draw(window);
}

void Renderer::drawGameOver(int winnerID, int player1Tanks, int player2Tanks, const std::vector<Button*>& buttons) {
    // título
    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(MENU_TITLE_FONT_SIZE);
    title.setFillColor(sf::Color::Red);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f - titleBounds.left,
        100.f
    );
    window.draw(title);

    // ganador
    sf::Text winnerText;
    winnerText.setFont(font);
    winnerText.setString("JUGADOR " + std::to_string(winnerID) + " GANA!");
    winnerText.setCharacterSize(HUD_TITLE_FONT_SIZE);
    winnerText.setFillColor(winnerID == 1 ? sf::Color::Blue : sf::Color::Yellow);
    sf::FloatRect winnerBounds = winnerText.getLocalBounds();
    winnerText.setPosition(
        (WINDOW_WIDTH - winnerBounds.width) / 2.f - winnerBounds.left,
        220.f
    );
    window.draw(winnerText);

    // estadísticas
    sf::Text statsTitle;
    statsTitle.setFont(font);
    statsTitle.setString("ESTADISTICAS");
    statsTitle.setCharacterSize(HUD_FONT_SIZE);
    statsTitle.setFillColor(sf::Color::White);
    sf::FloatRect statsBounds = statsTitle.getLocalBounds();
    statsTitle.setPosition(
        (WINDOW_WIDTH - statsBounds.width) / 2.f - statsBounds.left,
        320.f
    );
    window.draw(statsTitle);

    // tanques restantes jugador 1
    sf::Text player1Text;
    player1Text.setFont(font);
    player1Text.setString("Jugador 1: " + std::to_string(player1Tanks) + " tanques");
    player1Text.setCharacterSize(HUD_FONT_SIZE);
    player1Text.setFillColor(sf::Color::Blue);
    player1Text.setPosition(300.f, 390.f);
    window.draw(player1Text);

    // tanques restantes jugador 2
    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setString("Jugador 2: " + std::to_string(player2Tanks) + " tanques");
    player2Text.setCharacterSize(HUD_FONT_SIZE);
    player2Text.setFillColor(sf::Color::Yellow);
    player2Text.setPosition(300.f, 450.f);
    window.draw(player2Text);

    // botones
    for (auto& button : buttons)
        button->draw(window);
}

void Renderer::drawBackground() {
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(sf::Color(30, 30, 30));  // gris oscuro
    window.draw(background);
}
void Renderer::setUseSprites(bool useSprites) {
    this->useSprites = useSprites;
}

sf::Vector2f Renderer::cellToPixel(sf::Vector2i cell) const {
    return sf::Vector2f(
        cell.x * CELL_SIZE,
        cell.y * CELL_SIZE
    );
}

sf::Color Renderer::getTankColor(int playerID, int tankIndex) const {
    if (playerID == 1) {
        // jugador 1: azul y rojo
        if (tankIndex == 0 || tankIndex == 1)
            return sf::Color(0, 0, 255);    // azul
        else
            return sf::Color(255, 0, 0);    // rojo
    } else {
        // jugador 2: celeste y amarillo
        if (tankIndex == 0 || tankIndex == 1)
            return sf::Color(0, 255, 255);  // celeste
        else
            return sf::Color(255, 255, 0);  // amarillo
    }
}
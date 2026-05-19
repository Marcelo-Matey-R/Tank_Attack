#include <iostream>
#include "../include/Renderer.h"
#include "../include/Constants.h"
#include "../include/Button.h"
#include "../../Logic/include/Bullet.h"
#include "../../Logic/include/Map.h"

Renderer::Renderer(sf::RenderWindow &window, sf::Font &font) : window(window), font(font)
{
    useSprites = false;
}

void Renderer::drawMap(Map *map)
{
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            Position cell = {i, j};
            Element *elem = map->GetElementAt(cell);

            sf::RectangleShape tile(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            tile.setPosition(cellToPixel(cell));

            if (elem == nullptr)
            {
                // celda vacía — no debería pasar pero por seguridad
                tile.setFillColor(sf::Color(50, 50, 50));
            }
            else
            {
                switch (elem->GetType())
                {
                case TypeElement::Floor:
                    tile.setFillColor(sf::Color(100, 80, 60)); // marrón
                    tile.setOutlineColor(sf::Color(80, 60, 40));
                    tile.setOutlineThickness(0.5f);
                    break;
                case TypeElement::Obstacle:
                    tile.setFillColor(sf::Color(120, 120, 120)); // gris
                    tile.setOutlineColor(sf::Color(80, 80, 80));
                    tile.setOutlineThickness(0.5f);
                    break;
                case TypeElement::Tank:
                    // el suelo debajo del tanque
                    tile.setFillColor(sf::Color(100, 80, 60));
                    tile.setOutlineColor(sf::Color(80, 60, 40));
                    tile.setOutlineThickness(0.5f);
                    break;
                default:
                    tile.setFillColor(sf::Color(50, 50, 50));
                    break;
                }
            }

            window.draw(tile);
        }
    }
}

void Renderer::drawHoveredCell(sf::Vector2i cell)
{
    // verificar que la celda sea válida y esté dentro del mapa
    if (cell.x < 0 || cell.x >= GRID_COLS ||
        cell.y < 0 || cell.y >= GRID_ROWS)
        return;

    Position pos = {cell.y, cell.x};

    sf::RectangleShape hover(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    hover.setPosition(cellToPixel(pos));
    hover.setFillColor(sf::Color(255, 255, 255, 60)); // blanco semitransparente
    hover.setOutlineColor(sf::Color(255, 255, 255, 150));
    hover.setOutlineThickness(1.f);

    window.draw(hover);
}

void Renderer::drawTanks(Map* map) {
    for (int player = 0; player < 2; player++) {
        Tank** tanks = map->GetTanksOfPlayer(player);
        
        if (tanks == nullptr) {
           
            continue;
        }

        for (int i = 0; i < 4; i++) {  
            
            if (tanks[i] == nullptr) continue;
            
            
            if (!tanks[i]->IsAlive()) continue;

           
            Position pos = tanks[i]->GetPosition();


            sf::Color color = getTankColor(player + 1, i);
            drawTankShape(cellToPixel(pos), color, 0.f);
        }
    }
}

void Renderer::drawTankShape(sf::Vector2f position, sf::Color color, float angle)
{
    // cuerpo del tanque
    sf::RectangleShape body(sf::Vector2f(CELL_SIZE - 6, CELL_SIZE - 6));
    body.setFillColor(color);
    body.setOrigin((CELL_SIZE - 6) / 2.f, (CELL_SIZE - 6) / 2.f);
    body.setPosition(position.x + CELL_SIZE / 2.f,
                     position.y + CELL_SIZE / 2.f);
    body.setRotation(angle);
    window.draw(body);

    // cañón del tanque
    sf::RectangleShape cannon(sf::Vector2f(CELL_SIZE / 4.f, CELL_SIZE / 2.f));
    cannon.setFillColor(sf::Color(color.r * 0.7f,
                                  color.g * 0.7f,
                                  color.b * 0.7f));
    cannon.setOrigin(cannon.getSize().x / 2.f, cannon.getSize().y);
    cannon.setPosition(position.x + CELL_SIZE / 2.f,
                       position.y + CELL_SIZE / 2.f);
    cannon.setRotation(angle);
    window.draw(cannon);
}

void Renderer::drawSelectedTank(const Tank &tank)
{
    Position pos = tank.GetPosition();

    sf::RectangleShape selection(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    selection.setPosition(cellToPixel(pos));
    selection.setFillColor(sf::Color::Transparent);
    selection.setOutlineColor(sf::Color::White);
    selection.setOutlineThickness(2.f);
    window.draw(selection);
}

void Renderer::drawMainMenu(const std::vector<Button *> &buttons)
{
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
        150.f);

    window.draw(title);

    // botones
    for (auto &button : buttons)
        button->draw(window);
}

void Renderer::drawInstructions(const std::vector<Button *> &buttons)
{
    // título
    sf::Text title;
    title.setFont(font);
    title.setString("INSTRUCCIONES");
    title.setCharacterSize(MENU_TITLE_FONT_SIZE);
    title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f - titleBounds.left,
        50.f);
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
    for (auto &button : buttons)
        button->draw(window);
}

void Renderer::drawGameOver(int winnerID, int player1Tanks, int player2Tanks, const std::vector<Button *> &buttons)
{
    // título
    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(MENU_TITLE_FONT_SIZE);
    title.setFillColor(sf::Color::Red);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f - titleBounds.left,
        100.f);
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
        220.f);
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
        320.f);
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
    for (auto &button : buttons)
        button->draw(window);
}

void Renderer::drawBackground()
{
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(sf::Color(30, 30, 30)); // gris oscuro
    window.draw(background);
}

void Renderer::drawBullet(const Bullet &bullet)
{
    if (!bullet.IsActive())
        return;

    sf::Vector2f pos = bullet.GetCurrentPixelPosition();

    sf::CircleShape shape(BULLET_SIZE / 2.0f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(BULLET_SIZE / 2.0f, BULLET_SIZE / 2.0f);
    shape.setPosition(pos);

    window.draw(shape);
}

void Renderer::drawBulletPath(const Bullet &bullet)
{
    if (!bullet.IsActive())
        return;

    std::vector<Position> path = bullet.GetPath();

    for (const Position &cell : path)
    {
        sf::RectangleShape pathCell(sf::Vector2f(
            CELL_SIZE - PATH_CELL_PADDING,
            CELL_SIZE - PATH_CELL_PADDING));
        pathCell.setFillColor(sf::Color(255, 50, 50, PATH_COLOR_ALPHA));
        pathCell.setPosition(cellToPixel(cell));
        pathCell.move(PATH_CELL_PADDING / 2.0f, PATH_CELL_PADDING / 2.0f);
        window.draw(pathCell);
    }
}

void Renderer::drawPath(const std::vector<Position> &path, sf::Color color)
{
    for (const Position &cell : path)
    {
        sf::RectangleShape pathCell(sf::Vector2f(
            CELL_SIZE - PATH_CELL_PADDING,
            CELL_SIZE - PATH_CELL_PADDING));
        pathCell.setFillColor(color);
        pathCell.setPosition(cellToPixel(cell));
        pathCell.move(PATH_CELL_PADDING / 2.0f, PATH_CELL_PADDING / 2.0f);
        window.draw(pathCell);
    }
}

void Renderer::clearPaths()
{
    currentTankPath.clear();
    currentBulletPath.clear();
}

void Renderer::setUseSprites(bool useSprites)
{
    this->useSprites = useSprites;
}

sf::Vector2f Renderer::cellToPixel(sf::Vector2i cell) const
{
    return sf::Vector2f(
        cell.x * CELL_SIZE,
        cell.y * CELL_SIZE);
}

sf::Vector2f Renderer::cellToPixel(Position cell) const
{
    return sf::Vector2f(
        cell.j * CELL_SIZE,
        cell.i * CELL_SIZE);
}

sf::Color Renderer::getTankColor(int playerID, int tankIndex) const
{
    if (playerID == 1)
    {
        // jugador 1: azul y rojo
        if (tankIndex == 0 || tankIndex == 1)
            return sf::Color(0, 0, 255); // azul
        else
            return sf::Color(255, 0, 0); // rojo
    }
    else
    {
        // jugador 2: celeste y amarillo
        if (tankIndex == 0 || tankIndex == 1)
            return sf::Color(0, 255, 255); // celeste
        else
            return sf::Color(255, 255, 0); // amarillo
    }
}
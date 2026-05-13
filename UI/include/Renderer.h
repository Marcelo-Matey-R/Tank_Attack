#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.h"

//pa que la implementacion no joda
class Map;
class Tank;
class Bullet;

class Renderer {
public:
    Renderer(sf::RenderWindow& window, sf::Font& font);

    // Mapa
    void drawMap(const Map& map);
    void drawHoveredCell(sf::Vector2i cell);  // Para que se note la seleccion de casilla

    // Tanques
    void drawTanks(const std::vector<Tank>& tanks);
    void drawSelectedTank(const Tank& tank);  // para resaltar el tanque seleccionado

    // Bala
    void drawBullet(const Bullet& bullet);

    // Rutas
    void drawPath(const std::vector<sf::Vector2i>& path,
                  sf::Color color);           // para dibujan la ruta
    void drawBulletPath(const std::vector<sf::Vector2i>& path); // dibujar el camino de la bala
    void clearPaths();                        // borrar los caminos dibujados

    // Pantallas
    void drawMainMenu(const std::vector<class Button*>& buttons);
    void drawInstructions(const std::vector<class Button*>& buttons);
    void drawGameOver(int winnerID, int player1Tanks, int player2Tanks, const std::vector<Button*>& buttons);
    void drawBackground();

    // Texturas
    // esto se debe llamar una sola vez
    void loadTextures();

    // esto es para probar primero las ventanas
    void setUseSprites(bool useSprites);

private:
    sf::RenderWindow& window;
    sf::Font& font;
    bool useSprites;

    // texturas
    sf::Texture tankTextures[4];   // azul, celeste, rojo, amarillo
    sf::Texture tileTextures[2];   // piso, pared
    sf::Texture bulletTexture;
    sf::Texture powerUpTexture;

    // rutas a dibujar
    std::vector<sf::Vector2i> currentTankPath;
    std::vector<sf::Vector2i> currentBulletPath;

    // los helpers para dibujar
    void drawCellShape(sf::Vector2i cell, sf::Color color);
    void drawTankShape(sf::Vector2f position, sf::Color color, float angle);
    void drawTankSprite(sf::Vector2f position, int colorIndex, float angle);
    sf::Vector2f cellToPixel(sf::Vector2i cell) const;  // para pasar de una celda a los pixeles
    sf::Color getTankColor(int playerID, int tankIndex) const;
};
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
#include "Button.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "HUD.h"
#include "../Logic/include/Map.h"
#include "../Logic/include/Player.h"
#include "../Logic/include/Bullet.h"
#include "../Logic/include/Tank.h"

enum class GameState
{
    MainMenu,
    Instructions,
    Playing,
    GameOver
};

enum class TurnState
{
    SelectingTank,
    TankSelected,
    BulletMoving,
    GameFinished

};

class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    // Core
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    Bullet *currentBullet = nullptr; // bala activa actualmente
    std::vector<Position> currentTankPath;

    // Sistemas UI
    Renderer *renderer;
    InputHandler *inputHandler;
    HUD *hud;

    // Botones por pantalla
    std::vector<Button *> menuButtons;
    std::vector<Button *> instructionButtons;
    std::vector<Button *> gameOverButtons;

    // Game loop
    void handleEvents();
    void update();
    void render();

    // Logica de juego
    Map *map;
    Player *players[2];
    Tank *selectedTank;
    TurnState turnState;
    int currentPlayerIndex;
    sf::Clock gameClock;
    int timeRemaining;
    int winner;

    // Inicialización por pantalla
    void initMainMenu();
    void initInstructions();
    void initGameOver();
    void initPlaying();

    // Manejo de eventos por pantalla
    void handleMainMenuEvents(PlayerAction &action);
    void handleInstructionsEvents(PlayerAction &action);
    void handlePlayingEvents(PlayerAction &action);
    void handleGameOverEvents(PlayerAction &action);

    // Renderizado por pantalla
    void renderMainMenu();
    void renderInstructions();
    void renderPlaying();
    void renderGameOver();

    // Logica de playing
    void updateBullet();
    void updateTimer();
    void checkWinCondition();
    void endTurn();
    void handleTankSelection(Position cell);
    void handleTankMovement(Position cell);
    void handleTankAttack(Position cell);
    void handlePowerUp();

    // Helpers
    void clearButtons(std::vector<Button *> &buttons);
    void changeState(GameState newState);
    bool isTankFromCurrentPlayer(Tank *tank);
    int countAliveTanks(int playerIndex);
    PowerUp getAttackPowerUp();
    PowerUp getAccuracyPowerUp();
    PowerUp getMovementPowerUp();
};
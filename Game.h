#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
#include "Button.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "HUD.h"

enum class GameState {
    MainMenu,
    Instructions,
    Playing,
    GameOver
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    //Core
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;

    //Sistemas UI
    Renderer* renderer;
    InputHandler* inputHandler;
    HUD* hud;

    //Botones por pantalla
    std::vector<Button*> menuButtons;
    std::vector<Button*> instructionButtons;
    std::vector<Button*> gameOverButtons;

    //Game loop
    void handleEvents();
    void update();
    void render();

    //Inicialización por pantalla
    void initMainMenu();
    void initInstructions();
    void initGameOver();

    //Manejo de eventos por pantalla
    void handleMainMenuEvents(PlayerAction& action);
    void handleInstructionsEvents(PlayerAction& action);
    void handlePlayingEvents(PlayerAction& action);
    void handleGameOverEvents(PlayerAction& action);

    //Renderizado por pantalla
    void renderMainMenu();
    void renderInstructions();
    void renderPlaying();
    void renderGameOver();

    //Helpers
    void clearButtons(std::vector<Button*>& buttons);
    void changeState(GameState newState);

};
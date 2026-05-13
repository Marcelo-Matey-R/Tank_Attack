#include "Game.h"
#include "Constants.h"
#include "UI/include/Button.h"
#include "UI/include/Renderer.h"
#include "UI/include/InputHandler.h"
#include "UI/include/HUD.h"

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE)
{
    window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setKeyRepeatEnabled(false);

    font.loadFromFile("../" + FONTS_PATH + "PressStart2P-Regular.ttf");

    currentState = GameState::MainMenu;

    renderer = new Renderer(window, font);
    inputHandler = new InputHandler(window);
    hud = new HUD(window, font);

    initMainMenu();
}

Game::~Game()
{
    clearButtons(menuButtons);
    clearButtons(instructionButtons);
    clearButtons(gameOverButtons);
    delete renderer;
    delete inputHandler;
    delete hud;
}

void Game::run()
{
    while (window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents()
{
    PlayerAction action = inputHandler->processEvents();

    switch (currentState)
    {
    case GameState::MainMenu:
        handleMainMenuEvents(action);
        break;
    case GameState::Instructions:
        handleInstructionsEvents(action);
        break;
    case GameState::Playing:
        handlePlayingEvents(action);
        break;
    case GameState::GameOver:
        handleGameOverEvents(action);
        break;
    }
}

void Game::update()
{
    switch (currentState)
    {
    case GameState::MainMenu:
        for (auto &button : menuButtons)
            button->update(window);
        break;
    case GameState::Instructions:
        for (auto &button : instructionButtons)
            button->update(window);
        break;
    case GameState::GameOver:
        for (auto &button : gameOverButtons)
            button->update(window);
        break;
    default:
        break;
    }
}

void Game::render()
{
    window.clear(sf::Color::Black);

    switch (currentState)
    {
    case GameState::MainMenu:
        renderMainMenu();
        break;
    case GameState::Instructions:
        renderInstructions();
        break;
    case GameState::Playing:
        renderPlaying();
        break;
    case GameState::GameOver:
        renderGameOver();
        break;
    }

    window.display();
}

void Game::initMainMenu()
{
    clearButtons(menuButtons);
    float buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2.f;
    float startY = 350.f;
    float spacingY = MENU_BUTTON_SPACING + MENU_BUTTON_HEIGHT;
    menuButtons.push_back(new Button({buttonX, startY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Jugar", this->font, MENU_BUTTON_FONT_SIZE));
    menuButtons.push_back(new Button({buttonX, startY + spacingY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Instrucciones", this->font, MENU_BUTTON_FONT_SIZE));
    menuButtons.push_back(new Button({buttonX, startY + 2 * spacingY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Salir", this->font, MENU_BUTTON_FONT_SIZE));
}

void Game::initInstructions()
{
    clearButtons(instructionButtons);
    float buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2.f;
    instructionButtons.push_back(new Button(
        {buttonX, 700.f}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT},
        "Volver", font, MENU_BUTTON_FONT_SIZE));
}
void Game::initGameOver()
{
    clearButtons(gameOverButtons);
    float buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2.f;
    gameOverButtons.push_back(new Button(
        {buttonX, 550.f}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT},
        "Jugar de nuevo", font, MENU_BUTTON_FONT_SIZE));
    gameOverButtons.push_back(new Button(
        {buttonX, 620.f}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT},
        "Salir", font, MENU_BUTTON_FONT_SIZE));
}

void Game::handleMainMenuEvents(PlayerAction &action)
{
    if (action.type == ActionType::CloseWindow)
    {
        window.close();
        return;
    }
    sf::Event event = inputHandler->getLastEvent();
    if (action.type == ActionType::LeftClick)
    {
        if (menuButtons[0]->isClicked(window, event))
            changeState(GameState::Playing);
        else if (menuButtons[1]->isClicked(window, event))
            changeState(GameState::Instructions);
        else if (menuButtons[2]->isClicked(window, event))
            window.close();
    }
}
void Game::handleInstructionsEvents(PlayerAction &action)
{
    if (action.type == ActionType::CloseWindow)
    {
        window.close();
        return;
    }
    sf::Event event = inputHandler->getLastEvent();
    if (action.type == ActionType::LeftClick)
    {
        if (instructionButtons[0]->isClicked(window, event))
            changeState(GameState::MainMenu);
    }
}
void Game::handlePlayingEvents(PlayerAction &action) {}
void Game::handleGameOverEvents(PlayerAction &action)
{
    if (action.type == ActionType::CloseWindow)
    {
        window.close();
        return;
    }
    sf::Event event = inputHandler->getLastEvent();
    if (action.type == ActionType::LeftClick)
    {
        if (gameOverButtons[0]->isClicked(window, event))
            changeState(GameState::MainMenu);
        else if (gameOverButtons[1]->isClicked(window, event))
            window.close();
    }
}

void Game::renderMainMenu()
{
    renderer->drawBackground();
    renderer->drawMainMenu(menuButtons);
}

void Game::renderInstructions()
{
    renderer->drawBackground();
    renderer->drawInstructions(instructionButtons);
}

void Game::renderPlaying()
{
    renderer->drawBackground();
    hud->draw();
}

void Game::renderGameOver()
{
    renderer->drawBackground();
    renderer->drawGameOver(1, 3, 0, gameOverButtons);
}

void Game::clearButtons(std::vector<Button *> &buttons)
{
    for (auto i : buttons)
        delete i;
    buttons.clear();
}
void Game::changeState(GameState newState)
{
    currentState = newState;
    switch (newState)
    {
    case GameState::MainMenu:
        initMainMenu();
        break;
    case GameState::Instructions:
        initInstructions();
        break;
    case GameState::Playing:
        break;
    case GameState::GameOver:
        initGameOver();
        break;
    }
}
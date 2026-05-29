#include <iostream>
#include "include/Game.h"
#include "Constants.h"
#include "UI/include/Button.h"
#include "UI/include/Renderer.h"
#include "UI/include/InputHandler.h"
#include "UI/include/HUD.h"
#include "Logic/include/Map.h"
#include "Logic/include/Player.h"
#include "Logic/include/Bullet.h"
#include "Logic/include/Tank.h"

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
    map = nullptr;
    players[0] = nullptr;
    players[1] = nullptr;
    selectedTank = nullptr;
    currentBullet = nullptr;
    currentPlayerIndex = 0;
    turnState = TurnState::SelectingTank;
    timeRemaining = MAX_TIME_SECONDS;
    winner = -1;
    tankAnimStep = 0;
    tankAnimFrameCounter = 0;
    isTankMoving = false;
    tankAnimPath.Clear();
    initMainMenu();
}

Game::~Game()
{
    clearButtons(menuButtons);
    clearButtons(instructionButtons);
    clearButtons(gameOverButtons);
    if (currentBullet)
        delete currentBullet;
    if (players[0])
        delete players[0];
    if (players[1])
        delete players[1];
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
    case GameState::Playing:
        updateTimer();
        updateBullet();
        updateTankAnimation();
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
    menuButtons.PushBack(new Button({buttonX, startY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Jugar", this->font, MENU_BUTTON_FONT_SIZE));
    menuButtons.PushBack(new Button({buttonX, startY + spacingY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Instrucciones", this->font, MENU_BUTTON_FONT_SIZE));
    menuButtons.PushBack(new Button({buttonX, startY + 2 * spacingY}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT}, "Salir", this->font, MENU_BUTTON_FONT_SIZE));
}

void Game::initInstructions()
{
    clearButtons(instructionButtons);
    float buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2.f;
    instructionButtons.PushBack(new Button(
        {buttonX, 700.f}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT},
        "Volver", font, MENU_BUTTON_FONT_SIZE));
}

void Game::initPlaying()
{
    map = Map::GetInstance();

    map->RestartGame();

    if (players[0])
        delete players[0];
    if (players[1])
        delete players[1];
    players[0] = new Player(0);
    players[1] = new Player(1);

    selectedTank = nullptr;
    currentPlayerIndex = 0;
    turnState = TurnState::SelectingTank;
    timeRemaining = MAX_TIME_SECONDS;
    winner = -1;

    tankAnimStep = 0;
    tankAnimFrameCounter = 0;
    isTankMoving = false;
    tankAnimPath.Clear();

    if (currentBullet)
    {
        delete currentBullet;
        currentBullet = nullptr;
    }

    gameClock.restart();

    hud->updateCurrentPlayer(currentPlayerIndex + 1);
    hud->updateTimer(timeRemaining);

    for (int player = 0; player < 2; player++)
    {
        Tank **tanks = map->GetTanksOfPlayer(player);
        for (int i = 0; i < 4; i++)
        {
            if (tanks[i] != nullptr)
                hud->updateTankHealth(player + 1, i, 1.0f);
        }
    }
}

void Game::initGameOver()
{
    clearButtons(gameOverButtons);
    float buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2.f;
    gameOverButtons.PushBack(new Button(
        {buttonX, 550.f}, {MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT},
        "Volver al menu", font, MENU_BUTTON_FONT_SIZE));
    gameOverButtons.PushBack(new Button(
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
        {

            changeState(GameState::Playing);
        }
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
void Game::handlePlayingEvents(PlayerAction &action)
{
    if (action.type == ActionType::CloseWindow)
    {
        window.close();
        return;
    }

    // si la bala está en movimiento no aceptar inputs
    if (turnState == TurnState::BulletMoving)
        return;
    if (turnState == TurnState::GameFinished)
        return;

    switch (action.type)
    {
    case ActionType::LeftClick:
    {
        Position cell = {
            action.cellSelected.y,
            action.cellSelected.x};

        if (turnState == TurnState::SelectingTank)
        {
            handleTankSelection(cell);
        }
        else if (turnState == TurnState::TankSelected)
        {
            Element *elem = map->GetElementAt(cell);
            if (elem != nullptr &&
                elem->GetType() == TypeElement::Tank)
            {
                Tank *tank = map->GetTankIn(cell);
                if (tank != nullptr &&
                    isTankFromCurrentPlayer(tank))
                {
                    currentTankPath.Clear();
                    selectedTank = nullptr; 
                    handleTankSelection(cell);
                    return;
                }
            }
            handleTankMovement(cell);
        }
        break;
    }

    case ActionType::RightClick:
    {
        if (turnState != TurnState::TankSelected)
            return;

        Position cell = {
            action.cellSelected.y,
            action.cellSelected.x};
        handleTankAttack(cell);
        break;
    }

    case ActionType::ShiftPressed:
    {
        handlePowerUp();
        break;
    }

    default:
        break;
    }
}
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
    renderer->drawMap(map);
    renderer->drawHoveredCell(inputHandler->getHoveredCell());

   if (!currentTankPath.IsEmpty()) {
    Array<Position> remainingPath;
    for (int i = tankAnimStep; i < currentTankPath.Size(); i++)
        remainingPath.PushBack(currentTankPath[i]);
    renderer->drawPath(remainingPath, sf::Color(255, 255, 0, PATH_COLOR_ALPHA));
}


    if (selectedTank != nullptr)
        renderer->drawSelectedTank(*selectedTank);
    renderer->drawTanks(map);
    if (currentBullet && currentBullet->IsActive())
    {
        renderer->drawBulletPath(*currentBullet);
        renderer->drawBullet(*currentBullet);
    }
    hud->draw();
}

void Game::renderGameOver()
{
    renderer->drawBackground();
    renderer->drawGameOver(winner + 1, countAliveTanks(0), countAliveTanks(1), gameOverButtons);
}

void Game::updateTimer()
{
    // calcular tiempo restante
    int elapsed = (int)gameClock.getElapsedTime().asSeconds();
    timeRemaining = MAX_TIME_SECONDS - elapsed;

    // actualizar HUD
    hud->updateTimer(timeRemaining);

    // tiempo agotado
    if (timeRemaining <= 0)
    {
        timeRemaining = 0;
        checkWinCondition();
    }
}

void Game::updateBullet()
{
    if (!currentBullet)
        return;

    if (!currentBullet->IsActive())
    {
        turnState = TurnState::SelectingTank;
        endTurn();
        return;
    }

    // solo avanzar cada N frames
    bulletFrameCounter++;
    if (bulletFrameCounter < BULLET_FRAMES_PER_STEP)
        return;
    bulletFrameCounter = 0;

    currentBullet->Step();

    Position cell = currentBullet->GetCurrentCell();

    if (cell.i < 0 || cell.i >= GRID_ROWS ||
        cell.j < 0 || cell.j >= GRID_COLS)
    {
        currentBullet->Deactivate();
        turnState = TurnState::SelectingTank;
        endTurn();
        return;
    }

    Element *elem = map->GetElementAt(cell);
    if (elem == nullptr)
    {
        currentBullet->Deactivate();
        turnState = TurnState::SelectingTank;
        endTurn();
        return;
    }

    if (currentBullet->IsTank(map, cell))
    {
        Tank *tank = map->GetTankIn(cell);
        if (tank != nullptr)
        {
            PowerUp attackP = getAttackPowerUp();
            tank->DecreaseHealth(attackP);

            Tank **tanksP0 = map->GetTanksOfPlayer(0);
            Tank **tanksP1 = map->GetTanksOfPlayer(1);
            for (int i = 0; i < 4; i++)
            {
                if (tanksP0[i] != nullptr)
                    hud->updateTankHealth(1, i, tanksP0[i]->GetLife() / 100.f);
                if (tanksP1[i] != nullptr)
                    hud->updateTankHealth(2, i, tanksP1[i]->GetLife() / 100.f);
            }

            if (!tank->IsAlive())
                map->EliminateTank(tank, cell);
        }

        currentBullet->Deactivate();
        checkWinCondition();
        endTurn();
        return;
    }

    if (!currentBullet->IsActive())
    {
        turnState = TurnState::SelectingTank;
        endTurn();
    }
}

void Game::updateTankAnimation() {
    if (!isTankMoving) return;
    if (tankAnimPath.IsEmpty()) return;

    tankAnimFrameCounter++;
    if (tankAnimFrameCounter < TANK_FRAMES_PER_STEP) return;
    tankAnimFrameCounter = 0;

    tankAnimStep++;

    if (tankAnimStep >= (int)tankAnimPath.Size()) {
        isTankMoving = false;
        tankAnimPath.Clear();
        tankAnimStep = 0;
        endTurn();
        return;
    }

    // mover el tanque a la siguiente celda
    Position nextCell = tankAnimPath[tankAnimStep];
    map->MoveTank(selectedTank, nextCell);
    selectedTank = map->GetTankIn(nextCell);
}

void Game::checkWinCondition()
{
    int aliveTanksP0 = countAliveTanks(0);
    int aliveTanksP1 = countAliveTanks(1);

    // jugador 1 destruyó todos los tanques del jugador 2
    if (aliveTanksP1 == 0)
    {
        winner = 0;
        changeState(GameState::GameOver);
        return;
    }

    // jugador 2 destruyó todos los tanques del jugador 1
    if (aliveTanksP0 == 0)
    {
        winner = 1;
        changeState(GameState::GameOver);
        return;
    }

    // tiempo agotado — gana quien tenga más tanques
    if (timeRemaining <= 0)
    {
        if (aliveTanksP0 > aliveTanksP1)
            winner = 0;
        else if (aliveTanksP1 > aliveTanksP0)
            winner = 1;
        else
            winner = -1; // empate

        changeState(GameState::GameOver);
    }
}

void Game::endTurn()
{
    // consumir un turno del jugador actual
    players[currentPlayerIndex]->UseTurn();

    players[currentPlayerIndex]->ClearPowerUp();

    // limpiar trazos de la pantalla
    renderer->clearPaths();

    // limpiar bala
    if (currentBullet)
    {
        delete currentBullet;
        currentBullet = nullptr;
    }

    // limpiar tanque seleccionado
    selectedTank = nullptr;

    // verificar si el turno del jugador actual terminó
    if (players[currentPlayerIndex]->IsTurnOver())
    {
        // cambiar al otro jugador
        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
        players[currentPlayerIndex]->ResetTurn();

        // generar power-up aleatorio con cierta probabilidad
        std::random_device rng;
        std::mt19937 gen(rng());
        std::uniform_int_distribution<int> dis(1, 10);

        // 30% de probabilidad de recibir un power-up
        if (dis(gen) <= 3)
        {
            std::uniform_int_distribution<int> disPowerUp(1, 4);
            PowerUp randomPowerUp = static_cast<PowerUp>(disPowerUp(gen));
            players[currentPlayerIndex]->GivePower(randomPowerUp);

            // actualizar HUD con nuevos power-ups
            // por ahora solo mostramos cuántos tiene
            Array<std::string> powerUps;
            if (players[currentPlayerIndex]->HasPowerUp())
                powerUps.PushBack("Power-up disponible");
            hud->updatePowerUpQueue(currentPlayerIndex + 1, powerUps);
        }
    }

    // actualizar HUD
    hud->updateCurrentPlayer(currentPlayerIndex + 1);

    // resetear estado del turno
    turnState = TurnState::SelectingTank;
    currentTankPath.Clear();
}

void Game::handleTankSelection(Position cell)
{
    // verificar si hay un tanque en esa celda
    Element *elem = map->GetElementAt(cell);

    if (elem == nullptr)
        return;
    if (elem->GetType() != TypeElement::Tank)
        return;

    Tank *tank = map->GetTankIn(cell);
    if (tank == nullptr)
        return;

    // verificar que el tanque pertenezca al jugador actual
    if (!isTankFromCurrentPlayer(tank))
        return;

    // verificar que el tanque esté vivo
    if (!tank->IsAlive())
        return;

    // seleccionar el tanque
    selectedTank = tank;
    turnState = TurnState::TankSelected;
}

void Game::handleTankMovement(Position cell) {
    if (selectedTank == nullptr) return;

    Element* elem = map->GetElementAt(cell);
    if (elem == nullptr) return;
    if (elem->GetType() != TypeElement::Floor) return;

    PowerUp movePrecise = getMovementPowerUp();
    Element* mapGrid[GRID_ROWS][GRID_COLS];
    map->GetMap(mapGrid);

    Array<Position> path;
    try {
        path = selectedTank->GetPath(cell, mapGrid, movePrecise);
    } catch (...) {
        endTurn();
        return;
    }

    if (path.IsEmpty()) {
        endTurn();
        return;
    }

    // guardar path para animación
    currentTankPath = path;
    tankAnimPath = path;
    tankAnimStep = 0;
    tankAnimFrameCounter = 0;
    isTankMoving = true;
    
    // cambiar estado para bloquear inputs mientras se mueve
    turnState = TurnState::BulletMoving;
}

void Game::handleTankAttack(Position cell)
{
    if (selectedTank == nullptr)
        return;

    // verificar que no se dispare a la propia celda
    if (cell == selectedTank->GetPosition())
        return;

    if (currentBullet)
    {
        delete currentBullet;
        currentBullet = nullptr;
    }

    PowerUp attackPlus = getAttackPowerUp();
    PowerUp attackAccuracy = getAccuracyPowerUp();
    currentBullet = selectedTank->Attack(cell, attackPlus, attackAccuracy);

    // verificar que Attack() devolvió una bala válida
    if (currentBullet == nullptr)
    {
        std::cout << "Attack devolvio nullptr" << std::endl;
        return;
    }

    sf::Vector2f tankPixelPos = renderer->cellToPixel(selectedTank->GetPosition());
    sf::Vector2f destPixelPos = renderer->cellToPixel(cell);

    float dx = destPixelPos.x - tankPixelPos.x;
    float dy = destPixelPos.y - tankPixelPos.y;
    float angle = std::atan2(dy, dx) * 180.f / 3.14159f;

    // verificar que CalculatePath no crashee
    try
    {
        currentBullet->CalculatePath(map);
    }
    catch (...)
    {
        std::cout << "Error en CalculatePath" << std::endl;
        delete currentBullet;
        currentBullet = nullptr;
        return;
    }

    std::cout << "Path de bala size: " << currentBullet->GetPath().Size() << std::endl;
    std::cout << "Bala activa: " << currentBullet->IsActive() << std::endl;
    turnState = TurnState::BulletMoving;
}

void Game::handlePowerUp()
{
    // verificar que el jugador tenga power-ups disponibles
    if (!players[currentPlayerIndex]->HasPowerUp())
        return;

    // consumir y aplicar el power-up
    players[currentPlayerIndex]->ApplyPowerUp();

    // actualizar HUD
    Array<std::string> powerUps;
    if (players[currentPlayerIndex]->HasPowerUp())
        powerUps.PushBack("Power-up disponible");
    hud->updatePowerUpQueue(currentPlayerIndex + 1, powerUps);

    // el power-up consume el turno
    endTurn();
}

void Game::clearButtons(Array<Button *> &buttons)
{
    for (auto i : buttons)
        delete i;
    buttons.Clear();
}
void Game::changeState(GameState newState)
{
    currentState = newState;
    switch (newState)
    {
    case GameState::MainMenu:
        std::cout << "Iniciando MainMenu..." << std::endl;
        initMainMenu();
        std::cout << "MainMenu iniciado" << std::endl;
        break;
    case GameState::Instructions:
        std::cout << "Iniciando Instructions..." << std::endl;
        initInstructions();
        std::cout << "Instructions iniciado" << std::endl;
        break;
    case GameState::Playing:
        std::cout << "Iniciando Playing..." << std::endl;
        initPlaying();
        std::cout << "Playing iniciado" << std::endl;
        break;
    case GameState::GameOver:
        std::cout << "Iniciando GameOver..." << std::endl;
        initGameOver();
        std::cout << "GameOver iniciado" << std::endl;
        break;
    }
}

bool Game::isTankFromCurrentPlayer(Tank *tank)
{
    Tank **tanks = map->GetTanksOfPlayer(currentPlayerIndex);
    for (int i = 0; i < 4; i++)
    {
        if (tanks[i] == tank)
            return true;
    }
    return false;
}

int Game::countAliveTanks(int playerIndex)
{
    int count = 0;
    Tank **tanks = map->GetTanksOfPlayer(playerIndex);
    for (int i = 0; i < 4; i++)
    {
        if (tanks[i] != nullptr && tanks[i]->IsAlive())
            count++;
    }
    return count;
}
PowerUp Game::getAttackPowerUp()
{
    PowerUp p = players[currentPlayerIndex]->GetPowerUpInUse();
    if (p == PowerUp::ATTACKPOWER)
        return p;
    return PowerUp::NULL_POWERUP;
}

PowerUp Game::getAccuracyPowerUp()
{
    PowerUp p = players[currentPlayerIndex]->GetPowerUpInUse();
    if (p == PowerUp::ATTACKACCURACY)
        return p;
    return PowerUp::NULL_POWERUP;
}

PowerUp Game::getMovementPowerUp()
{
    PowerUp p = players[currentPlayerIndex]->GetPowerUpInUse();
    if (p == PowerUp::MOVEMENTPRECISION)
        return p;
    return PowerUp::NULL_POWERUP;
}

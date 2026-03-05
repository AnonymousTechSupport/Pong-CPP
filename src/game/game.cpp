#include "game.h"
#include "utils/input.h"
#include "utils/logger.h"
#include <windows.h>

bool Game::Init(Window& window)
{
    Logger::Get().LogInfo("Initializing Game");
    Logger::Get().LogInfo("_______________________________");

    m_window = &window;
    if (!window.IsRunning())
    {
        Logger::Get().LogError("Window is not running");
        return false;
    }
    if (!m_renderer.Init(&window))
    {
        Logger::Get().LogError("Failed to initialize renderer");
        return false;
    }

    m_timer.Start();

    InitializeEntities();
    m_initialised = true;

    Logger::Get().LogInfo("[GAME INITIALIZATION COMPLETE]");
    return true;
}

void Game::Run(Window& window)
{
    if (!m_initialised)
    {
        Logger::Get().LogWarning("Attempted to run uninitialized game");
        return;
    }

    Logger::Get().LogInfo("[GAME LOOP STARTING]");
    while (window.IsRunning())
    {
        if (!window.ProcessMessages())
            break;

        Input::Get().Update();

        double dt = m_timer.Tick();
        Update(dt);
        m_renderer.Render();
    }
    Logger::Get().LogInfo("[GAME LOOP ENDED]");
}

void Game::Update(double dt)
{
    m_totalTime += dt;
    ++m_frameCount;

    // ########################
    // # CHECKING FOR INPUT   #
    // ########################
    if (Input::Get().IsKeyDown('W'))
        m_player->y -= m_player->velocity * static_cast<float>(dt);
    if (Input::Get().IsKeyDown('S'))
        m_player->y += m_player->velocity * static_cast<float>(dt);

    // ########################
    // # RENDERING ENTITIES   #
    // ########################
    m_renderer.DrawPlayer(m_player->x, m_player->y, m_player->w, m_player->h);
    m_renderer.DrawEnemy(m_enemy->x, m_enemy->y, m_enemy->w, m_enemy->h);

    // ########################
    // # CHECKING GAME STATE  #
    // ########################
    if (m_totalTime < 1.0)
        return;
    m_fps = static_cast<double>(m_frameCount) / m_totalTime;
    Logger::Get().LogEngineState(m_fps, m_frameCount, m_totalTime);
    GetEntityList();
    m_frameCount = 0;
    m_totalTime = 0.0;
}

void Game::GetEntityList()
{
    Logger::Get().LogDebug("[ENTITY LIST]");
    Logger::Get().LogDebug("_______________________________");

    Logger::Get().LogDebug(
        "[PLAYER ENTITY]  X: " + std::to_string((int)m_player->x) +
        ", Y: " + std::to_string((int)m_player->y));
    Logger::Get().LogDebug(
        "[ENEMY ENTITY]  X: " + std::to_string((int)m_enemy->x) +
        ", Y: " + std::to_string((int)m_enemy->y));
}

void Game::Shutdown()
{
    Logger::Get().LogInfo("[GAME SHUTTING DOWN]");
    m_renderer.Shutdown();
    m_initialised = false;
    Logger::Get().LogInfo("[GAME SHUTDOWN COMPLETE]");
}

Game::Game() : m_initialised(false) {}

void Game::InitializeEntities()
{
    Logger::Get().LogDebug("Initializing game entities");
    Logger::Get().LogDebug("_______________________________");

    // INIT PLAYER ENTITY
    // #########################
    Logger::Get().LogDebug("Initializing Player entity");
    m_player = std::make_unique<Player>(Player{30.0f, (float)m_window->GetHeight() / 2.0f});

    if (!m_player)
    {
        Logger::Get().LogError("Failed to initialize Player entity");
        return;
    }

    // INIT ENEMY ENTITY
    // #########################
    Logger::Get().LogDebug("Initializing Enemy entity");
    m_enemy = std::make_unique<Enemy>(
        Enemy{(float)m_window->GetWidth() - 60.0f, (float)m_window->GetHeight() / 2.0f});

    if (!m_enemy)
    {
        Logger::Get().LogError("Failed to initialize Enemy entity");
        return;
    }

    Logger::Get().LogDebug("[ENTITIES INITIALIZATION COMPLETE]");
}

Game::~Game()
{
    // Dont have to call reset here since the dstructor will do that
    // just for safety bcs why not lol

    Logger::Get().LogDebug("Game destructor called - destroying entities");
    Shutdown();
    m_player.reset();
    Logger::Get().LogDebug("Player destroyed");
    m_enemy.reset();
    Logger::Get().LogDebug("Enemy destroyed");
}

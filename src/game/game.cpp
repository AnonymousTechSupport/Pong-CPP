#include "game.h"
#include "utils/input.h"
#include "utils/logger.h"

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
        Logger::Get().LogWarning(
            "Attempted to run uninitialized game");
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

    // input handling
    if (Input::Get().IsKeyDown('W'))
        m_player->pos.y -=
            m_player->velocity * static_cast<float>(dt);
    if (Input::Get().IsKeyDown('S'))
        m_player->pos.y +=
            m_player->velocity * static_cast<float>(dt);

    // enqueue entities for drawing
    RectUtil playerRect{
        m_player->pos.x,
        m_player->pos.y,
        m_player->size.x,
        m_player->size.y,
        m_player->color};
    m_renderer.DrawEntity(playerRect);

    RectUtil enemyRect{
        m_enemy->pos.x,
        m_enemy->pos.y,
        m_enemy->size.x,
        m_enemy->size.y,
        m_enemy->color};
    m_renderer.DrawEntity(enemyRect);

    // update fps timer
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
    Logger::Get().LogDebug(
        "Player at (" +
        std::to_string(static_cast<int>(m_player->pos.x)) + "," +
        std::to_string(static_cast<int>(m_player->pos.y)) + ")");
    Logger::Get().LogDebug(
        "Enemy  at (" +
        std::to_string(static_cast<int>(m_enemy->pos.x)) + "," +
        std::to_string(static_cast<int>(m_enemy->pos.y)) + ")");
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

    // initialize player entity
    Logger::Get().LogDebug("Initializing Player entity");
    m_player = std::make_unique<Player>();
    m_player->pos = {
        30.0f, static_cast<float>(m_window->GetHeight()) / 2.0f};
    m_player->size = {20.0f, 100.0f};
    m_player->color = {0.2f, 0.7f, 0.2f};

    // initialize enemy entity
    Logger::Get().LogDebug("Initializing Enemy entity");
    m_enemy = std::make_unique<Enemy>();
    m_enemy->pos = {
        static_cast<float>(m_window->GetWidth()) - 60.0f,
        static_cast<float>(m_window->GetHeight()) / 2.0f};
    m_enemy->size = {20.0f, 100.0f};
    m_enemy->color = {0.8f, 0.2f, 0.2f};

    Logger::Get().LogDebug("[ENTITIES INITIALIZATION COMPLETE]");
}

Game::~Game()
{
    Logger::Get().LogDebug("Game destructor");
    Shutdown();
}

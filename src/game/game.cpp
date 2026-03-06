#include "game.h"
#include "game/entity/ball.h"
#include "game/entity/enemy.h"
#include "game/entity/player.h"
#include "utils/input.h"
#include "utils/logger.h"

int Game::Run()
{
    // public entry point used by main().
    if (!Initialize())
        return -1;

    RunLoop();
    return 0;
}

bool Game::Initialize()
{
    Logger::Get().LogInfo("[GAME INITIALIZATION STARTING]");

    if (!m_window.IsRunning())
    {
        Logger::Get().LogError("Window is not running");
        return false;
    }
    if (!m_renderer.Initialize(&m_window))
    {
        Logger::Get().LogError("Failed to initialize renderer");
        return false;
    }

    m_timer.Start();

    InitializeEntities();
    m_initialized = true;

    Logger::Get().LogInfo("[GAME INITIALIZATION COMPLETE]");
    return true;
}

void Game::RunLoop()
{
    if (!m_initialized)
    {
        Logger::Get().LogWarning(
            "Attempted to run uninitialized game");
        return;
    }

    Logger::Get().LogInfo("[GAME LOOP STARTING]");
    while (m_window.IsRunning())
    {
        if (!m_window.ProcessMessages())
            break;

        Input::Get().Update();

        double deltaTime = m_timer.Tick();
        Update(deltaTime);
        m_renderer.RenderFrame();
    }
    Logger::Get().LogInfo("[GAME LOOP ENDED]");
}

void Game::Update(double deltaTime)
{
    m_totalTime += deltaTime;
    ++m_frameCount;

    // update and enqueue all entities
    for (auto& e : m_entities)
    {
        e->Update(deltaTime);

        RenderVariant cmd = e->GetRenderCommand();
        m_renderer.QueueRenderCommand(cmd);
    }

    // update fps timer
    if (m_totalTime < 1.0)
        return;
    m_fps = static_cast<double>(m_frameCount) / m_totalTime;
    Logger::Get().LogEngineState(m_fps, m_frameCount, m_totalTime);
    LogEntityCount();
    m_frameCount = 0;
    m_totalTime = 0.0;
}

void Game::LogEntityCount()
{
    // simple debug output showing number of entities
    Logger::Get().LogDebug(
        "Entity count: " + std::to_string(m_entities.size()));
}

void Game::Shutdown()
{
    Logger::Get().LogInfo("[GAME SHUTTING DOWN]");
    m_renderer.Shutdown();
    m_initialized = false;
}

Game::Game() : m_initialized(false) {}

void Game::InitializeEntities()
{
    Logger::Get().LogDebug("[CREATING INITIAL ENTITIES]");
    m_entities.clear();

    // create a player paddle at startup
    Logger::Get().LogDebug("[CREATING PLAYER ENTITY]");
    m_entities.emplace_back(std::make_unique<Player>(m_window));

    // create an enemy paddle at startup
    Logger::Get().LogDebug("[CREATING ENEMY ENTITY]");
    m_entities.emplace_back(std::make_unique<Enemy>(m_window));

    // create the ball in the center
    Logger::Get().LogDebug("[CREATING BALL ENTITY]");
    m_entities.emplace_back(std::make_unique<Ball>(m_window));

    Logger::Get().LogDebug("[ENTITY CREATION COMPLETE]");
}

Game::~Game()
{
    Logger::Get().LogDebug("[GAME DESTRUCTOR CALLED]");
    if (m_initialized)
    {
        Shutdown();
    }
}

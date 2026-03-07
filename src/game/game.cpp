#include "game.h"
#include "game/entity/ball.h"
#include "game/entity/enemy.h"
#include "game/entity/player.h"
#include "utils/input.h"
#include "utils/logger.h"
#include <SDL3/SDL.h>

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
    LOG_INFO("[GAME INITIALIZATION STARTING]");
    // Initialize SDL here and create the window
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        LOG_ERROR(std::string("SDL_Init failed: ") + SDL_GetError());
        return false;
    }

    if (!m_window.Create())
    {
        LOG_ERROR("Failed to create window");
        return false;
    }
    if (!m_renderer.Initialize())
    {
        LOG_ERROR("Failed to initialize renderer");
        return false;
    }

    m_timer.Start();

    InitializeEntities();
    m_initialized = true;

    LOG_INFO("[GAME INITIALIZATION COMPLETE]");
    return true;
}

void Game::RunLoop()
{
    if (!m_initialized)
    {
        LOG_WARNING("Attempted to run uninitialized game");
        return;
    }

    LOG_INFO("[GAME LOOP STARTING]");
    while (window::IsRunning(&m_window))
    {
        if (!window::PollEvents(&m_window))
            break;

        input::Update();

        double deltaTime = m_timer.Tick();
        Update(deltaTime);
        m_renderer.RenderFrame();
    }
    LOG_INFO("[GAME LOOP ENDED]");
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
    LOG_ENGINE_STATE(m_fps, m_frameCount, m_totalTime);
    LogEntityCount();
    m_frameCount = 0;
    m_totalTime = 0.0;
}

void Game::LogEntityCount()
{
    // simple debug output showing number of entities
    LOG_DEBUG("Entity count: " + std::to_string(m_entities.size()));
}

void Game::Shutdown()
{
    LOG_INFO("[GAME SHUTTING DOWN]");
    m_renderer.Shutdown();
    m_window.Shutdown();
    m_initialized = false;
}

Game::Game() : m_renderer(&m_window), m_initialized(false) {}

void Game::InitializeEntities()
{
    LOG_DEBUG("[CREATING INITIAL ENTITIES]");
    m_entities.clear();

    // create a player paddle at startup
    LOG_DEBUG("[CREATING PLAYER ENTITY]");
    m_entities.emplace_back(std::make_unique<Player>(m_window));

    // create an enemy paddle at startup
    LOG_DEBUG("[CREATING ENEMY ENTITY]");
    m_entities.emplace_back(std::make_unique<Enemy>(m_window));

    // create the ball in the center
    LOG_DEBUG("[CREATING BALL ENTITY]");
    m_entities.emplace_back(std::make_unique<Ball>(m_window));

    LOG_DEBUG("[ENTITY CREATION COMPLETE]");
}

Game::~Game()
{
    LOG_DEBUG("[GAME DESTRUCTOR CALLED]");
    if (m_initialized)
    {
        Shutdown();
    }
    // Only call SDL_Quit() if SDL was initialized earlier
    if (SDL_WasInit(0) != 0)
    {
        SDL_Quit();
        LOG_INFO("SDL_Quit() called from Game destructor");
    }
}

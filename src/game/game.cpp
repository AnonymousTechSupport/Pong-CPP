#include "game.h"
#include "utils/input/input.h"
#include "utils/logger/logger.h"
#include <SDL3/SDL.h>

// --- Game implementation
// ---------------------------------------------------------

Game::Game() : m_renderer(&m_window)
{
}

Game::~Game()
{
    LOG_DEBUG("[GAME DESTRUCTOR CALLED]");
    if (m_initialized)
        Shutdown();

    if (SDL_WasInit(0) != 0)
    {
        SDL_Quit();
    }
}

int Game::Run()
{
    if (!Initialize())
        return -1;

    RunLoop();
    return 0;
}

bool Game::Initialize()
{
    LOG_INFO("[GAME INITIALIZING]");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        LOG_ERROR("SDL_Init failed: {}", SDL_GetError());
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

    // Create a test entity
    {
        Entity* e = m_entityManager.CreateEntity();
        TransformComponent t;
        t.position = {400.0f, 300.0f};
        t.scale = {50.0f, 50.0f};
        m_entityManager.AddTransform(e, t);

        RenderComponent r;
        r.shape = ShapeType::Ball;
        r.color = {0.2f, 0.7f, 0.3f};
        m_entityManager.AddRender(e, r);

        InputComponent i;
        i.isControllable = false;
        m_entityManager.AddInput(e, i);
    }

    m_timer.Start();
    m_initialized = true;

    LOG_INFO("[GAME INITIALIZED]");
    return true;
}

void Game::RunLoop()
{
    if (!m_initialized)
    {
        LOG_WARNING("RunLoop called before Initialize");
        return;
    }

    LOG_INFO("[GAME LOOP STARTING]");
    while (m_window.IsRunning())
    {
        if (!m_window.ProcessEvent())
            break;

        Input::Get().Update();

        double deltaTime = m_timer.Tick();
        m_entityManager.UpdateAll(static_cast<float>(deltaTime));

        Update(deltaTime);

        m_entityManager.DrawAll(&m_renderer);

        m_renderer.RenderFrame();
    }
    LOG_INFO("[GAME LOOP ENDED]");
}

void Game::Update(double deltaTime)
{
    m_totalTime += deltaTime;
    m_frameCount += 1;

    // Log FPS once per second.
    if (m_totalTime < 1.0)
        return;

    m_fps = static_cast<double>(m_frameCount) / m_totalTime;
    LOG_ENGINE_STATE(m_fps, m_frameCount, m_totalTime);

    m_frameCount = 0;
    m_totalTime = 0.0;
}

void Game::Shutdown()
{
    LOG_INFO("[GAME SHUTTING DOWN]");

    m_renderer.Shutdown();
    m_window.Shutdown();

    m_initialized = false;
}

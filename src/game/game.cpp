#include "game.h"
#include "platform/iwindow.h"
#include "renderer/irenderer.h"
#include "utils/iinput.h"
#include "utils/logger.h"
#include <SDL3/SDL.h>
#include <string>

// ------------------------------------------------------------------------------
// ------------------------------ GAME IMPLEMENTATION ------------------------------
// ------------------------------------------------------------------------------

// --- Implementations
// --------------------

Game::Game() : m_renderer(&m_window)
{
}

// --- Destructor -------------------------------------
Game::~Game()
{
    LOG_DEBUG("[GAME DESTRUCTOR CALLED]");
    if (m_initialized)
        Shutdown();

    if (SDL_WasInit(0) != 0)
    {
        SDL_Quit();
        LOG_INFO("SDL_Quit called");
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
        LOG_ERROR(std::string("SDL_Init failed: ") + std::string(SDL_GetError()));
        return false;
    }

    // Use the interface pointer so this code is backend-agnostic.
    IWindow* window = &m_window;
    if (!window->Create())
    {
        LOG_ERROR("Failed to create window");
        return false;
    }

    IRenderer* renderer = &m_renderer;
    if (!renderer->Initialize())
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

        // Make the demo entity controllable via input
        InputComponent ic;
        ic.isControllable = true;
        ic.speed = 360.0f;
        m_entityManager.AddInput(e, ic);
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

    IWindow* window = &m_window;
    IRenderer* renderer = &m_renderer;

    LOG_INFO("[GAME LOOP STARTING]");
    while (window->IsRunning())
    {
        if (!window->ProcessEvent())
            break;

        input::Update();

        double deltaTime = m_timer.Tick();
        // Update entities
        m_entityManager.UpdateAll(static_cast<float>(deltaTime));

        Update(deltaTime);

        // Queue entity renders
        m_entityManager.DrawAll(renderer);

        renderer->RenderFrame();
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
    m_frameCount = 0;
    m_totalTime = 0.0;

    LOG_ENGINE_STATE(m_fps, m_frameCount, m_totalTime);
}

void Game::Shutdown()
{
    LOG_INFO("[GAME SHUTTING DOWN]");

    IRenderer* renderer = &m_renderer;
    renderer->Shutdown();

    IWindow* window = &m_window;
    window->Shutdown();

    m_initialized = false;
}

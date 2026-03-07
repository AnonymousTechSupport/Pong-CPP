#pragma once
#include "game/entities/entity_handler.h"
#include "platform/sdl_window.h"
#include "renderer/renderer.h"
#include "utils/timer.h"

// --- Game (top-level coordinator)
// ---------------------------------------

class Game
{
  public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game();

    int Run();

  private:
    bool Initialize();
    void RunLoop();
    void Update(double deltaTime);
    void Shutdown();

  private:
    Window m_window;
    Renderer m_renderer;
    Timer m_timer;
    EntityManager m_entityManager;

    bool m_initialized = false;
    double m_totalTime = 0.0;
    int m_frameCount = 0;
    double m_fps = 0.0;
};

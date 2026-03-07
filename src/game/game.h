#pragma once
#include "game/entity/entity.h"
#include "platform/sdl_window.h"
#include "renderer/renderer.h"
#include "utils/timer.h"

#include <memory>
#include <vector>

class Game
{
  public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    ~Game();

    bool Initialize(); // create internal window

    void RunLoop();
    int Run(); // initialize + loop
    void Update(double deltaTime);

  private:
    void Shutdown();

  public:
    void LogEntityCount(); // debug helper

  private:
    void InitializeEntities();

  private:
    Window m_window;
    Renderer m_renderer;
    Timer m_timer;
    bool m_initialized =
        false; // true after successful initialization

  private:
    // generic container for all game entities
    std::vector<std::unique_ptr<GameEntity>> m_entities;

  private:
    double m_totalTime = 0.0;
    int m_frameCount = 0;
    double m_fps = 0.0;
};
#pragma once
#include "renderer.h"
#include "utils/timer.h"
#include "win32_window.h"

#include <memory>

class Game
{
  public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    ~Game();

    bool Init(class Window& window);

    void Run(class Window& window);
    void Update(double dt);
    void Shutdown();

  public:
    void GetEntityList(); // Just for debugging at this point

  private:
    void InitializeEntities();

  private:
    Renderer m_renderer;
    Timer m_timer;
    Window* m_window = nullptr;
    bool m_initialised = false;

  private:
    // PLAYER ENTITY
    // #########################
    struct Player
    {
        float x;
        float y;
        float w = 20.0f;
        float h = 100.0f;
        int velocity = 400; // pixels per second
    };
    std::unique_ptr<Player> m_player;

    // ENEMY ENTITY
    // #########################
    struct Enemy
    {
        float x;
        float y;
        float w = 20.0f;
        float h = 100.0f;
        int velocity = 400; // pixels per second
    };
    std::unique_ptr<Enemy> m_enemy;

  private:
    double m_totalTime = 0.0;
    int m_frameCount = 0;
    double m_fps = 0.0;
};
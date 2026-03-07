#pragma once
#include "platform/sdl_window.h"
#include "utils/math_utils.h"
#include <variant>
#include <vector>

class Renderer
{
  public:
    Renderer(Window* window);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    ~Renderer();

    bool Initialize();
    void RenderFrame();

    void QueueRenderRectangle(const RenderRectangle& rect);
    void QueueRenderCommand(const RenderVariant& cmd);

    void Shutdown();

  private:
    void DrawRectangle(const RenderRectangle& r);
    void DrawCircle(const RenderBall& ball);

  private:
    // SDL-managed OpenGL context
    SDL_GLContext m_glContext = nullptr;

    Window* m_window = nullptr;

    float m_width = 0.0f;
    float m_height = 0.0f;

  private:
    // per-frame queue of render commands (use the global
    // RenderVariant)
    std::vector<RenderVariant> m_renderQueue;
};

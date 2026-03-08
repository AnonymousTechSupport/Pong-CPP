#pragma once
#include "platform/iwindow.h"
#include "renderer/irenderer.h"
#include <SDL3/SDL.h>
#include <vector>

// ------------------------------------------------------------------------------
// -------------------------- RENDERER (OPENGL BACKEND) --------------------------
// ------------------------------------------------------------------------------

class Renderer : public IRenderer
{
  public:
    // --- Lifecycle / Main API
    explicit Renderer(IWindow* window);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer() override;

    bool Initialize() override;
    void Shutdown() override;
    void RenderFrame() override;

    // Queueing API
    void QueueRenderRectangle(const RenderRectangle& rect) override;
    void QueueRenderBall(const RenderBall& cmd) override;

  private:
    // Private helpers
    void DrawRectangle(const RenderRectangle& r);
    void DrawCircle(const RenderBall& ball);

  private:
    // Private state
    IWindow* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    float m_width = 0.0f;
    float m_height = 0.0f;

    std::vector<RenderVariant> m_renderQueue;
};

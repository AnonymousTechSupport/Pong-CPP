#pragma once
#include "platform/interface/iwindow.h"
#include "renderer/interface/irenderer.h"
#include "renderer/interface/render_types.h"
#include "utils/math/math_utils.h"
#include <SDL3/SDL.h>
#include <vector>

class OpenGLRenderer : public IRenderer
{
  public:
    explicit OpenGLRenderer(IWindow* window);
    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
    ~OpenGLRenderer() override;

    bool Initialize() override;
    void Shutdown() override;
    void RenderFrame() override;

    void QueueRenderRectangle(const RenderRectangle& rect) override;
    void QueueRenderBall(const RenderBall& cmd) override;

  private:
    void DrawRectangle(const RenderRectangle& r);
    void DrawCircle(const RenderBall& ball);

  private:
    IWindow* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    float m_width = 0.0f;
    float m_height = 0.0f;

    std::vector<RenderVariant> m_renderQueue;
};

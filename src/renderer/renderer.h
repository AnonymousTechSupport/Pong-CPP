#pragma once
#include "platform/win32_window.h"
#include "utils/math_utils.h"
#include <variant>
#include <vector>
#include <windows.h>

class Renderer
{
  public:
    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    ~Renderer();

    bool Initialize(Window* window);
    void RenderFrame();

    void QueueRenderRectangle(const RenderRectangle& rect);
    void QueueRenderCommand(const RenderVariant& cmd);

    void Shutdown();

  private:
    void DrawRectangle(const RenderRectangle& r);
    void DrawCircle(const RenderBall& ball);

  private:
    HDC m_deviceContext = nullptr; // handle to device context
    HGLRC m_openglContext =
        nullptr; // handle to OpenGL rendering context
    Window* m_window = nullptr;

    float m_width = 0.0f;
    float m_height = 0.0f;

  private:
    // per-frame queue of rectangles to render
    using RenderVariant = std::variant<RenderRectangle, RenderBall>;
    std::vector<RenderVariant> m_renderQueue;
};
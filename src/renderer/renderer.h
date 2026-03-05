#pragma once
#include "platform/win32_window.h"
#include "utils/math_utils.h"
#include <vector>
#include <windows.h>

class Renderer
{
  public:
    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    ~Renderer();

    bool Init(Window* window);
    void Render();

    void DrawEntity(const RectUtil& entity);

    void Shutdown();

  private:
    void DrawRect(const RectUtil& r);

  private:
    HDC m_hdc = nullptr;
    HGLRC m_glrc = nullptr;
    Window* m_window = nullptr;

    float m_width = 0.0f;
    float m_height = 0.0f;

  private:
    // per-frame list of rectangles to draw
    std::vector<RectUtil> m_drawEntities;
};
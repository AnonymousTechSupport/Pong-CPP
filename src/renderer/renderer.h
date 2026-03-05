#pragma once
#include "platform/win32_window.h"
#include <vector>
#include <windows.h>

class Renderer
{
  public:
    struct Color
    {
        float r, g, b;
        Color(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f) : r(_r), g(_g), b(_b) {}
    };
    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    ~Renderer();

    bool Init(Window* window);
    void Render();

    void
    DrawPlayer(float x, float y, float w, float h, const Color& color = Color(0.2f, 0.7f, 0.2f));
    void
    DrawEnemy(float x, float y, float w, float h, const Color& color = Color(0.8f, 0.2f, 0.2f));

    void Shutdown();

  private:
    HDC m_hdc = nullptr;
    HGLRC m_glrc = nullptr;
    Window* m_window = nullptr;

    float m_width = 0.0f;
    float m_height = 0.0f;

  private:
    // Making a list of entity rectangles to draw each frame, we can batch render them in the
    // render method
    struct Rect
    {
        float x, y, w, h;
        Color col;
    };
    std::vector<Rect> m_drawEntities;
};
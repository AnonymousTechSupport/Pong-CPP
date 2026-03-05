#include "renderer.h"
#include <GL/gl.h>

Renderer::Renderer() = default;

bool Renderer::Init(Window* window)
{
    m_window = window;
    if (!m_window)
    {
        MessageBoxW(nullptr, L"Invalid window pointer", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    m_hdc = GetDC(m_window->GetHwnd());
    if (!m_hdc)
    {
        MessageBoxW(nullptr, L"Failed to get device context", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (pixelFormat == 0)
    {
        MessageBoxW(nullptr, L"Failed to choose pixel format", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (!SetPixelFormat(m_hdc, pixelFormat, &pfd))
    {
        MessageBoxW(nullptr, L"Failed to set pixel format", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    m_glrc = wglCreateContext(m_hdc);
    if (!m_glrc)
    {
        MessageBoxW(nullptr, L"Failed to create OpenGL context", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (!wglMakeCurrent(m_hdc, m_glrc))
    {
        MessageBoxW(
            nullptr, L"Failed to make OpenGL context current", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // Get the window dimensions
    RECT rect{};
    if (GetClientRect(m_window->GetHwnd(), &rect) == 0)
    {
        MessageBoxW(nullptr, L"Failed to get window dimensions", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    m_width = rect.right - rect.left;
    m_height = rect.bottom - rect.top;

    return true;
}

void Renderer::Render()
{
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up an orthographic projection in pixel coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_width, m_height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw queued rects
    for (const auto& cmd : m_drawEntities)
    {
        glColor3f(cmd.col.r, cmd.col.g, cmd.col.b);
        glBegin(GL_QUADS);
        glVertex2f(cmd.x, cmd.y);
        glVertex2f(cmd.x + cmd.w, cmd.y);
        glVertex2f(cmd.x + cmd.w, cmd.y + cmd.h);
        glVertex2f(cmd.x, cmd.y + cmd.h);
        glEnd();
    }

    // Once the frame clears, we clear the entity vector ready for next frame
    // simply flushes draw queue
    m_drawEntities.clear();

    SwapBuffers(m_hdc);
}

// #################################
// # DRAW FUNCTIONS FOR ENTITIES   #
// #################################
void Renderer::DrawPlayer(float x, float y, float w, float h, const Color& color)
{
    m_drawEntities.emplace_back(Rect{x, y, w, h, color});
}

void Renderer::DrawEnemy(float x, float y, float w, float h, const Color& color)
{
    m_drawEntities.emplace_back(Rect{x, y, w, h, color});
}

void Renderer::Shutdown()
{
    if (m_glrc)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_glrc);
        m_glrc = nullptr;
    }

    if (m_hdc)
    {
        if (m_window)
            ReleaseDC(m_window->GetHwnd(), m_hdc);
        else
            ReleaseDC(WindowFromDC(m_hdc), m_hdc);
        m_hdc = nullptr;
    }
    m_window = nullptr;
}

Renderer::~Renderer()
{
    Shutdown();
}

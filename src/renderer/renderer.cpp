#include "renderer.h"
#include "math_utils.h"
#include <GL/gl.h>
#include <cmath>

Renderer::Renderer() = default;

bool Renderer::Initialize(Window* window)
{
    m_window = window;
    if (!m_window)
    {
        MessageBoxW(
            nullptr,
            L"Invalid window pointer",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    m_deviceContext = GetDC(m_window->GetHwnd());
    if (!m_deviceContext)
    {
        MessageBoxW(
            nullptr,
            L"Failed to get device context",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
    if (pixelFormat == 0)
    {
        MessageBoxW(
            nullptr,
            L"Failed to choose pixel format",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    if (!SetPixelFormat(m_deviceContext, pixelFormat, &pfd))
    {
        MessageBoxW(
            nullptr,
            L"Failed to set pixel format",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    m_openglContext = wglCreateContext(m_deviceContext);
    if (!m_openglContext)
    {
        MessageBoxW(
            nullptr,
            L"Failed to create OpenGL context",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    if (!wglMakeCurrent(m_deviceContext, m_openglContext))
    {
        MessageBoxW(
            nullptr,
            L"Failed to make OpenGL context current",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    // Get the window dimensions
    RECT rect{};
    if (GetClientRect(m_window->GetHwnd(), &rect) == 0)
    {
        MessageBoxW(
            nullptr,
            L"Failed to get window dimensions",
            L"Error",
            MB_OK | MB_ICONERROR);
        return false;
    }

    m_width = rect.right - rect.left;
    m_height = rect.bottom - rect.top;

    return true;
}

void Renderer::RenderFrame()
{
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up an orthographic projection in pixel
    // coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_width, m_height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw queued commands
    for (const auto& cmd : m_renderQueue)
    {
        if (auto* rect = std::get_if<RenderRectangle>(&cmd))
        {
            DrawRectangle(*rect);
        }
        else if (auto* ball = std::get_if<RenderBall>(&cmd))
        {
            DrawCircle(*ball);
        }
    }

    // Once the frame clears, we clear the render queue
    // ready for next frame
    m_renderQueue.clear();

    SwapBuffers(m_deviceContext);
}

// #################################
// # DRAW FUNCTIONS FOR ENTITIES   #
// #################################

void Renderer::QueueRenderRectangle(const RenderRectangle& rect)
{
    m_renderQueue.emplace_back(rect);
}

void Renderer::QueueRenderCommand(const RenderVariant& cmd)
{
    m_renderQueue.emplace_back(cmd);
}

void Renderer::DrawRectangle(const RenderRectangle& r)
{
    glColor3f(r.color.r, r.color.g, r.color.b);
    glBegin(GL_QUADS);
    glVertex2f(r.pos.x, r.pos.y);
    glVertex2f(r.pos.x + r.size.x, r.pos.y);
    glVertex2f(r.pos.x + r.size.x, r.pos.y + r.size.y);
    glVertex2f(r.pos.x, r.pos.y + r.size.y);
    glEnd();
}

void Renderer::DrawCircle(const RenderBall& ball)
{
    const int segments = 32;
    glColor3f(ball.color.r, ball.color.g, ball.color.b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball.center.x, ball.center.y);
    for (int i = 0; i <= segments; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = ball.radius * cosf(theta);
        float y = ball.radius * sinf(theta);
        glVertex2f(ball.center.x + x, ball.center.y + y);
    }
    glEnd();
}

void Renderer::Shutdown()
{
    if (m_openglContext)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_openglContext);
        m_openglContext = nullptr;
    }

    if (m_deviceContext)
    {
        if (m_window)
            ReleaseDC(m_window->GetHwnd(), m_deviceContext);
        else
            ReleaseDC(WindowFromDC(m_deviceContext), m_deviceContext);
        m_deviceContext = nullptr;
    }
    m_window = nullptr;
}

Renderer::~Renderer()
{
    Shutdown();
}

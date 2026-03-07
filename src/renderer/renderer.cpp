#include "renderer.h"
#include "logger.h"
#include "math_utils.h"
#if defined(_WIN32) || defined(_WIN64)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include <GL/gl.h>

Renderer::Renderer(Window* window) : m_window(window) {}

bool Renderer::Initialize()
{
    if (!m_window)
    {
        return false;
    }

    SDL_Window* sdlWindow = m_window->GetSDLWindow();
    if (!sdlWindow)
    {
        LOG_ERROR(
            "Renderer::Initialize - invalid SDL window pointer");
        return false;
    }

    // Request a compatibility/profile that supports fixed-function
    // (glBegin)
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    m_glContext = SDL_GL_CreateContext(sdlWindow);
    if (!m_glContext)
    {
        std::string err(SDL_GetError());
        LOG_ERROR(
            std::string("Failed to create SDL GL context: ") + err);
        return false;
    }

    if (!SDL_GL_MakeCurrent(sdlWindow, m_glContext))
    {
        std::string err(SDL_GetError());
        LOG_ERROR(
            std::string("Failed to make SDL GL context current: ") +
            err);
        return false;
    }

    // Get the window dimensions from SDL
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(sdlWindow, &width, &height);
    m_width = static_cast<float>(width);
    m_height = static_cast<float>(height);

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

    SDL_Window* sdlWindow = m_window->GetSDLWindow();
    if (sdlWindow)
        SDL_GL_SwapWindow(sdlWindow);
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
    const int segments = 48;
    const float cx = ball.center.x;
    const float cy = ball.center.y;
    const float r = ball.radius;
    const float twoPi = 6.28318530717958647692f; // 2 * PI

    glColor3f(ball.color.r, ball.color.g, ball.color.b);
    glBegin(GL_TRIANGLE_FAN);
    // center vertex
    glVertex2f(cx, cy);

    // outer vertices
    for (int i = 0; i <= segments; ++i)
    {
        float t =
            static_cast<float>(i) / static_cast<float>(segments);
        float theta = t * twoPi;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        glVertex2f(x, y);
    }

    glEnd();
}

void Renderer::Shutdown()
{
    if (m_glContext)
    {
        SDL_GL_MakeCurrent(nullptr, nullptr);
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
    m_window = nullptr;
}

Renderer::~Renderer()
{
    Shutdown();
}

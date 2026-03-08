#include "opengl_renderer.h"
#include "utils/logger/logger.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <type_traits>
#include <variant>

#if defined(_WIN32) || defined(_WIN64)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <GL/gl.h>
#include <string>

OpenGLRenderer::OpenGLRenderer(IWindow* window) : m_window(window)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
    Shutdown();
}

bool OpenGLRenderer::Initialize()
{
    if (!m_window)
    {
        LOG_ERROR("Renderer::Initialize — no window provided");
        return false;
    }

    SDL_Window* sdlWindow = static_cast<SDL_Window*>(m_window->GetNativeWindow());
    if (!sdlWindow)
    {
        LOG_ERROR("Renderer::Initialize — GetNativeWindow returned null");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    m_glContext = SDL_GL_CreateContext(sdlWindow);
    if (!m_glContext)
    {
        LOG_ERROR(std::string("Failed to create GL context: ") + SDL_GetError());
        return false;
    }

    if (!SDL_GL_MakeCurrent(sdlWindow, m_glContext))
    {
        LOG_ERROR(std::string("Failed to make GL context current: ") + SDL_GetError());
        return false;
    }

    m_width = static_cast<float>(m_window->GetWidth());
    m_height = static_cast<float>(m_window->GetHeight());

    return true;
}

void OpenGLRenderer::RenderFrame()
{
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_width, m_height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (const auto& cmd : m_renderQueue)
    {
        std::visit(
            [this](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, RenderRectangle>)
                    DrawRectangle(arg);
                else if constexpr (std::is_same_v<T, RenderBall>)
                    DrawCircle(arg);
            },
            cmd);
    }

    m_renderQueue.clear();

    SDL_Window* sdlWindow = static_cast<SDL_Window*>(m_window->GetNativeWindow());
    if (sdlWindow)
        SDL_GL_SwapWindow(sdlWindow);
}

void OpenGLRenderer::Shutdown()
{
    if (m_glContext)
    {
        SDL_GL_MakeCurrent(nullptr, nullptr);
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
    m_window = nullptr;
}

void OpenGLRenderer::QueueRenderRectangle(const RenderRectangle& rect)
{
    m_renderQueue.emplace_back(rect);
}

void OpenGLRenderer::QueueRenderBall(const RenderBall& cmd)
{
    m_renderQueue.emplace_back(cmd);
}

void OpenGLRenderer::DrawRectangle(const RenderRectangle& r)
{
    glColor3f(r.color.r, r.color.g, r.color.b);
    glBegin(GL_QUADS);
    glVertex2f(r.pos.x, r.pos.y);
    glVertex2f(r.pos.x + r.size.x, r.pos.y);
    glVertex2f(r.pos.x + r.size.x, r.pos.y + r.size.y);
    glVertex2f(r.pos.x, r.pos.y + r.size.y);
    glEnd();
}

void OpenGLRenderer::DrawCircle(const RenderBall& ball)
{
    constexpr int segments = 48;
    constexpr float twoPi = 6.28318530717958647692f;

    glColor3f(ball.color.r, ball.color.g, ball.color.b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball.center.x, ball.center.y);
    for (int i = 0; i <= segments; ++i)
    {
        float theta = (static_cast<float>(i) / static_cast<float>(segments)) * twoPi;
        glVertex2f(ball.center.x + ball.radius * std::cosf(theta),
                   ball.center.y + ball.radius * std::sinf(theta));
    }
    glEnd();
}

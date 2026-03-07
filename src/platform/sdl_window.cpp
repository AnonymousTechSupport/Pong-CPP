#include "sdl_window.h"
#include "utils/input.h"
#include "utils/logger.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <string>

// ------------------------------------------------------------------------------
// --------------------------- SDL WINDOW IMPLEMENTATION -------------------------
// ------------------------------------------------------------------------------

Window::Window() = default;
Window::~Window()
{
    Shutdown();
}

bool Window::Create()
{
    SDL_DisplayID displayId = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayId);

    int winW = 1920;
    int winH = 1080;

    if (displayMode)
    {
        winW = displayMode->w;
        winH = displayMode->h;
    }
    else
    {
        LOG_WARNING("Could not query display mode, falling back to "
                    "1920x1080");
    }

    m_window = SDL_CreateWindow("Engine", winW, winH, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

    if (!m_window)
    {
        LOG_ERROR(std::string("Failed to create SDL window: ") + SDL_GetError());
        m_running = false;
        return false;
    }

    LOG_INFO("SDL window created successfully");
    SDL_RaiseWindow(m_window);
    return true;
}

bool Window::ProcessEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // Forward all SDL events to the input system before checking
        // for quit.
        Input::Get().ProcessSDLInput(event);

        if (event.type == SDL_EVENT_QUIT)
        {
            m_running = false;
            return false;
        }
    }
    return true;
}

void Window::Shutdown()
{
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

// ------------------------------------------------------------------------------
// ---------------------------- GETTERS / SETTERS -------------------------------
// ------------------------------------------------------------------------------

bool Window::IsRunning() const
{
    return m_running;
}

// --- Getters / Setters
// ---------------------

int Window::GetWidth() const
{
    if (!m_window)
    {
        LOG_ERROR("GetWidth called with null SDL_Window");
        return 0;
    }
    int w = 0, h = 0;
    SDL_GetWindowSize(m_window, &w, &h);
    return w;
}

int Window::GetHeight() const
{
    if (!m_window)
    {
        LOG_ERROR("GetHeight called with null SDL_Window");
        return 0;
    }
    int w = 0, h = 0;
    SDL_GetWindowSize(m_window, &w, &h);
    return h;
}

void* Window::GetNativeWindow() const
{
    return m_window;
}

HWND Window::GetWindowHandle() const
{
    if (!m_window)
    {
        LOG_ERROR("GetWindowHandle called with null SDL_Window");
        return nullptr;
    }

    SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    if (props == 0)
    {
        LOG_ERROR("Failed to get SDL window properties");
        return nullptr;
    }

    void* hwnd = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

    if (!hwnd)
    {
        LOG_WARNING("Could not retrieve native HWND from SDL window "
                    "properties");
        return nullptr;
    }

    return static_cast<HWND>(hwnd);
}

// ---------------------

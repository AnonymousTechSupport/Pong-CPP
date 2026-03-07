#pragma once
#include <SDL3/SDL.h>

// allows us to not include windows.h header
#if defined(_WIN32) || defined(_WIN64)
typedef struct HWND__* HWND;
#else
using HWND = void*;
#endif

class Window
{
  public:
    Window();
    bool Create();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();

    bool ProcessEvent();
    void Shutdown();
    bool isRunning() const;

  public:
    HWND GetWindowHandle() const;
    SDL_Window* GetSDLWindow() const;
    int GetWidth() const;
    int GetHeight() const;

  private:
    SDL_Window* m_window = nullptr;
    bool m_running = true;
};

namespace window
{
static inline bool IsRunning(const Window* w) noexcept
{
    return w && w->isRunning();
}

static inline HWND Handle(const Window* w) noexcept
{
    return w ? w->GetWindowHandle() : nullptr;
}

static inline int Width(const Window* w) noexcept
{
    return w ? w->GetWidth() : 0;
}

static inline int Height(const Window* w) noexcept
{
    return w ? w->GetHeight() : 0;
}

static inline bool PollEvents(Window* w) noexcept
{
    return w ? w->ProcessEvent() : false;
}

static inline void Shutdown(Window* w) noexcept
{
    if (w)
        w->Shutdown();
}
} // namespace window

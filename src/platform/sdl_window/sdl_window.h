// --- SDL Window implementation (concrete)
// ------------------------------------------

#pragma once
#include "platform/interface/iwindow.h"
#include <SDL3/SDL.h>

// Windows-only: forward-declare HWND without pulling in windows.h.
#if defined(_WIN32) || defined(_WIN64)
typedef struct HWND__* HWND;
#else
using HWND = void*;
#endif

class SDLWindow : public IWindow
{
  public:
    SDLWindow();
    SDLWindow(const SDLWindow&) = delete;
    SDLWindow& operator=(const SDLWindow&) = delete;
    ~SDLWindow() override;

    bool Create() override;
    void Shutdown() override;
    bool ProcessEvent() override;

    bool IsRunning() const override;
    int GetWidth() const override;
    int GetHeight() const override;

    void* GetNativeWindow() const override;

    HWND GetWindowHandle() const;

  private:
    SDL_Window* m_window = nullptr;
    bool m_running = true;
};

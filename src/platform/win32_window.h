#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
  public:
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    ~Window();

    bool ProcessMessages();
    void Shutdown();

    bool IsRunning() const;

    HWND GetHwnd() const;

    int GetWidth() const { return 1280; }
    int GetHeight() const { return 720; }

  private:
    HINSTANCE m_instance;
    HWND m_hWindow;
    bool m_isRunning;
    bool m_registeredClass = false;
};
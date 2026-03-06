#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

    int GetWidth() const
    {
        if (m_hWindow)
        {
            RECT rect;
            if (GetClientRect(m_hWindow, &rect))
                return rect.right - rect.left;
        }
        return 0;
    }

    int GetHeight() const
    {
        if (m_hWindow)
        {
            RECT rect;
            if (GetClientRect(m_hWindow, &rect))
                return rect.bottom - rect.top;
        }
        return 0;
    }

  private:
    HINSTANCE m_instance;
    HWND m_hWindow;
    bool m_isRunning;
    bool m_registeredClass = false;
};
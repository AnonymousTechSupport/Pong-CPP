#pragma once

#include "logger.h"
#include <array>
#include <bitset>
#include <windows.h>

class Input
{
  public:
    enum class MouseButton : int
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    static Input& Get();

    // Call once per frame to advance state
    void Update();

    // Feed Win32 messages (call from WindowProc)
    void ProcessWin32Message(UINT message, WPARAM wParam, LPARAM lParam);

    // Queries
    bool IsKeyDown(int vk) const;
    bool IsKeyPressed(int vk) const; // true only on the frame it was pressed

    POINT GetMousePos() const;
    void GetMousePos(int& x, int& y) const; // convenience wrapper

    bool IsMouseDown(MouseButton b) const;
    bool IsMouseDown(int button) const; // convenience wrapper

    // Clear all input state (useful on focus loss)
    void Clear();

  private:
    Input();

    std::bitset<256> m_keys;
    std::bitset<256> m_prevKeys;

    std::array<bool, 3> m_mouseButtons;
    POINT m_mousePos{0, 0};
};

// Inline implementation to make Input header-only (avoids separate .o linking issues)

inline Input& Input::Get()
{
    static Input s_instance;
    return s_instance;
}

inline Input::Input()
{
    m_keys.reset();
    m_prevKeys.reset();
    m_mouseButtons.fill(false);
    m_mousePos.x = 0;
    m_mousePos.y = 0;
}

inline void Input::Update()
{
    m_prevKeys = m_keys;
}

inline void Input::Clear()
{
    m_keys.reset();
    m_prevKeys.reset();
    m_mouseButtons.fill(false);
}

inline void Input::ProcessWin32Message(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        unsigned char vk = static_cast<unsigned char>(wParam & 0xFF);
        m_keys.set(vk);
        // debug log
        Logger::Get().LogDebug(std::string("Input: KEYDOWN vk=") + std::to_string((unsigned)vk));
        break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        unsigned char vk = static_cast<unsigned char>(wParam & 0xFF);
        m_keys.reset(vk);
        Logger::Get().LogDebug(std::string("Input: KEYUP vk=") + std::to_string((unsigned)vk));
        break;
    }
    case WM_MOUSEMOVE:
    {
        int x = static_cast<int>(static_cast<short>(lParam & 0xFFFF));
        int y = static_cast<int>(static_cast<short>((lParam >> 16) & 0xFFFF));
        m_mousePos.x = x;
        m_mousePos.y = y;
        break;
    }
    case WM_LBUTTONDOWN:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Left)] = true;
        break;
    case WM_LBUTTONUP:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Left)] = false;
        break;
    case WM_RBUTTONDOWN:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Right)] = true;
        break;
    case WM_RBUTTONUP:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Right)] = false;
        break;
    case WM_MBUTTONDOWN:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Middle)] = true;
        break;
    case WM_MBUTTONUP:
        m_mouseButtons[static_cast<int>(Input::MouseButton::Middle)] = false;
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
            Clear();
        break;
    default:
        break;
    }
}

inline bool Input::IsKeyDown(int vk) const
{
    if (vk < 0 || vk > 255)
        return false;
    return m_keys.test(static_cast<unsigned char>(vk));
}

inline bool Input::IsKeyPressed(int vk) const
{
    if (vk < 0 || vk > 255)
        return false;
    unsigned char u = static_cast<unsigned char>(vk);
    return m_keys.test(u) && !m_prevKeys.test(u);
}

inline POINT Input::GetMousePos() const
{
    return m_mousePos;
}

inline void Input::GetMousePos(int& x, int& y) const
{
    x = m_mousePos.x;
    y = m_mousePos.y;
}

inline bool Input::IsMouseDown(MouseButton b) const
{
    return m_mouseButtons[static_cast<int>(b)];
}

inline bool Input::IsMouseDown(int button) const
{
    if (button < 0 || button > 2)
        return false;
    return m_mouseButtons[button];
}

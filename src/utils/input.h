#pragma once

#include "utils/math_utils.h"
#include <SDL3/SDL.h>
#include <array>
#include <bitset>

class Input
{
  public:
    enum class MouseButton : int
    {
        Left,
        Right,
        Middle,
    };

  public:
    static Input& Get();
    void Update();

    void ProcessSDLInput(const SDL_Event& event);

    bool isKeyDown(int key) const;
    bool isMouseButtonDown(MouseButton button) const;

    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;

    void SetMousePosition(int x, int y);
    Vector2 GetMousePosition() const;

    void Clear();

  private:
    Input();
    std::bitset<512> m_keys;
    std::bitset<512> m_prevKeys;
    std::array<bool, 3> m_mouseButtons;

    int m_mouseX;
    int m_mouseY;
};

namespace input
{
static inline bool IsKeyDown(int key) noexcept
{
    return Input::Get().isKeyDown(key);
}

static inline bool IsKeyPressed(int key) noexcept
{
    return Input::Get().isKeyPressed(key);
}

static inline bool IsKeyReleased(int key) noexcept
{
    return Input::Get().isKeyReleased(key);
}

static inline bool
IsMouseButtonDown(Input::MouseButton button) noexcept
{
    return Input::Get().isMouseButtonDown(button);
}

static inline Vector2 GetMousePosition() noexcept
{
    return Input::Get().GetMousePosition();
}

static inline void SetMousePosition(int x, int y) noexcept
{
    Input::Get().SetMousePosition(x, y);
}

static inline void Clear() noexcept
{
    Input::Get().Clear();
}

static inline void Update() noexcept
{
    Input::Get().Update();
}
} // namespace input

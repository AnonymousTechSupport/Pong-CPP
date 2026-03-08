#pragma once
#include "utils/math/math_utils.h"

class IInput
{
  public:
    enum class MouseButton : int
    {
        Left,
        Right,
        Middle,
    };

    virtual ~IInput() = default;

    virtual void Update() = 0;

    virtual bool IsKeyDown(int key) const = 0;
    virtual bool IsKeyPressed(int key) const = 0;
    virtual bool IsKeyReleased(int key) const = 0;

    virtual bool IsMouseButtonDown(MouseButton button) const = 0;

    virtual Vector2 GetMousePosition() const = 0;
    virtual void SetMousePosition(int x, int y) = 0;

    virtual void Clear() = 0;

    static IInput& Get();
};

namespace input
{
inline void Update() noexcept
{
    IInput::Get().Update();
}
inline bool IsKeyDown(int key) noexcept
{
    return IInput::Get().IsKeyDown(key);
}
inline bool IsKeyPressed(int key) noexcept
{
    return IInput::Get().IsKeyPressed(key);
}
inline bool IsKeyReleased(int key) noexcept
{
    return IInput::Get().IsKeyReleased(key);
}
inline bool IsMouseButtonDown(IInput::MouseButton b) noexcept
{
    return IInput::Get().IsMouseButtonDown(b);
}
inline Vector2 GetMousePosition() noexcept
{
    return IInput::Get().GetMousePosition();
}
inline void SetMousePosition(int x, int y) noexcept
{
    IInput::Get().SetMousePosition(x, y);
}
inline void Clear() noexcept
{
    IInput::Get().Clear();
}
} // namespace input

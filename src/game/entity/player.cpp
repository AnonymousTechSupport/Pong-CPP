#include "player.h"
#include "input.h"
#include "platform/win32_window.h"

Player::Player(const Window& window)
{
    pos = {30.0f, static_cast<float>(window.GetHeight()) / 2.0f};
    size = {20.0f, 100.0f};
    color = {0.2f, 0.7f, 0.2f};
    speed = 360.0f;
}

void Player::Update(double dt)
{
    // input handling
    if (Input::Get().IsKeyDown('W'))
        pos.y -= speed * static_cast<float>(dt);
    if (Input::Get().IsKeyDown('S'))
        pos.y += speed * static_cast<float>(dt);
}

RenderVariant Player::GetRenderCommand() const
{
    return RenderRectangle{pos, size, color, static_cast<int>(speed)};
}
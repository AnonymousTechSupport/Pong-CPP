#include "player.h"
#include "input.h"
#include "platform/sdl_window.h"

Player::Player(const Window& window)
{
    pos = {30.0f, static_cast<float>(window::Height(&window)) / 2.0f};
    size = {20.0f, 100.0f};
    color = {0.2f, 0.7f, 0.2f};
    speed = 360.0f;
}

void Player::Update(double dt)
{
    // input handling
    if (input::IsKeyDown(SDL_SCANCODE_W))
        pos.y -= speed * static_cast<float>(dt);
    if (input::IsKeyDown(SDL_SCANCODE_S))
        pos.y += speed * static_cast<float>(dt);
}

RenderVariant Player::GetRenderCommand() const
{
    return RenderRectangle{pos, size, color, static_cast<int>(speed)};
}

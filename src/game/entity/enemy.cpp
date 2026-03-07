#include "enemy.h"

#include "platform/sdl_window.h"

Enemy::Enemy(const Window& window)
{
    pos = {
        static_cast<float>(window::Width(&window) - 60.0f),
        static_cast<float>(window::Height(&window)) / 2.0f};
    size = {20.0f, 100.0f};
    color = {0.2f, 0.7f, 0.2f};
    speed = 360.0f;
}

void Enemy::Update(double dt)
{
    // no implementation yet
}

RenderVariant Enemy::GetRenderCommand() const
{
    return RenderRectangle{pos, size, color, static_cast<int>(speed)};
}

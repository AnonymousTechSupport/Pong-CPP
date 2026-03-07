#include "ball.h"
#include "platform/sdl_window.h"

Ball::Ball(const Window& window)
{
    windowWidth = window::Width(&window);
    windowHeight = window::Height(&window);
    pos = {
        static_cast<float>(windowWidth) / 2.0f,
        static_cast<float>(windowHeight) / 2.0f};
    radius = 10.0f;
    color = {0.9f, 0.1f, 0.1f};
    speed = 300.0f;

    velocity = {1.0f, 1.0f};
}

void Ball::Update(double /*dt*/)
{
    // static ball – no movement or bouncing for now
    // (velocity and speed fields remain unused)
}

RenderVariant Ball::GetRenderCommand() const
{
    return RenderBall{pos, radius, color, static_cast<int>(speed)};
}

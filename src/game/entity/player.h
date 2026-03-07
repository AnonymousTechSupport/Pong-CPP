#pragma once
#include "entity.h"
#include "platform/sdl_window.h"

struct Player : public GameEntity
{
    Vector2 pos, size;
    ColorUtil color;
    float speed; // pixels per second

    Player(const Window& window);

    void Update(double dt) override;
    RenderVariant GetRenderCommand() const override;
};

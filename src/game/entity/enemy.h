#pragma once
#include "entity.h"
#include "platform/sdl_window.h"

struct Enemy : public GameEntity
{
    Vector2 pos, size;
    ColorUtil color;
    float speed; // pixels per second

    Enemy(const Window& window);

    void Update(double dt) override;
    RenderVariant GetRenderCommand() const override;
};

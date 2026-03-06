#pragma once
#include "entity.h"
#include "platform/win32_window.h"

struct Ball : public GameEntity
{
    Vector2 pos;
    Vector2 velocity;
    float radius;
    ColorUtil color;
    float speed; // pixels per second magnitude

    int windowWidth;
    int windowHeight;

    Ball(const Window& window);

    void Update(double dt) override;
    RenderVariant GetRenderCommand() const override;
};
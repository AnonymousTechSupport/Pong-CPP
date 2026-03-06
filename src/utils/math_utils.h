#pragma once

#include <variant>

struct Vector2
{
    float x;
    float y;
};

struct ColorUtil
{
    float r, g, b;
    ColorUtil(float red = 1.0f, float green = 1.0f, float blue = 1.0f)
        : r(red), g(green), b(blue)
    {
    }
};

struct RenderRectangle
{
    Vector2 pos;
    Vector2 size;
    ColorUtil color;
    int speed;
};

struct RenderBall
{
    Vector2 center;
    float radius;
    ColorUtil color;
    int speed;
};

// generic render command type used throughout the engine
using RenderVariant = std::variant<RenderRectangle, RenderBall>;

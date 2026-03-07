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
    ColorUtil(float red = 1.0f, float green = 1.0f, float blue = 1.0f) : r(red), g(green), b(blue)
    {
    }
};

// ------------------------------------------------------------------------------
// ------------------------------ MATH UTILITIES ---------------------------------
// ------------------------------------------------------------------------------

// Render data types
struct RenderRectangle
{
    Vector2 pos;
    Vector2 size;
    ColorUtil color;
};

struct RenderBall
{
    Vector2 center;
    float radius;
    ColorUtil color;
};

// Generic render command type submitted to IRenderer each frame.
using RenderVariant = std::variant<RenderRectangle, RenderBall>;

// ------------------

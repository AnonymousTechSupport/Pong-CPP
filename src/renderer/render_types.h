// --- Renderer data types
// -------------------------------------------------
// Render commands queued each frame and flushed by IRenderer::RenderFrame().

#pragma once
#include "utils/math_utils.h"
#include <variant>

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

// Generic render command submitted to IRenderer each frame.
using RenderVariant = std::variant<RenderRectangle, RenderBall>;

#pragma once

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

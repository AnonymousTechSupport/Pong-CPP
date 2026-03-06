#pragma once
#include "utils/math_utils.h"

struct GameEntity
{
    GameEntity() = default;
    virtual ~GameEntity() = default;

    virtual void Update(double dt) = 0;

    // return a variant containing whatever renderable the entity
    // wants to
    virtual RenderVariant GetRenderCommand() const = 0;
};

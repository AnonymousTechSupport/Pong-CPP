#include "entity_handler.h"
#include "utils/input/input.h"
#include "utils/logger/logger.h"

// --- EntityManager systems ---

void EntityManager::UpdateAll(float dt)
{
    for (auto& entity : m_entities)
    {
        if (!entity->input || !entity->transform)
            continue;

        auto* t = entity->transform.get();
        auto* ic = entity->input.get();

        if (!ic->isControllable)
            continue;

        // Movement uses simple ASCII letter checks: 'W'/'A'/'S'/'D'.
        // Input resolves these to scancodes internally so game code stays SDL-free.
        if (input::IsKeyDown('W'))
            t->position.y -= ic->speed * dt;
        if (input::IsKeyDown('S'))
            t->position.y += ic->speed * dt;
        if (input::IsKeyDown('A'))
            t->position.x -= ic->speed * dt;
        if (input::IsKeyDown('D'))
            t->position.x += ic->speed * dt;
    }
}

void EntityManager::DrawAll(IRenderer* renderer)
{
    if (!renderer)
    {
        LOG_ERROR("DrawAll called with null renderer");
        return;
    }

    for (const auto& entity : m_entities)
    {
        if (!entity->render || !entity->transform)
            continue;

        const auto& transform = *entity->transform;
        const auto& render = *entity->render;

        if (render.shape == ShapeType::Rectangle)
        {
            RenderRectangle r;
            r.pos = {transform.position.x - transform.scale.x / 2.0f,
                     transform.position.y - transform.scale.y / 2.0f};
            r.size = transform.scale;
            r.color = render.color;
            renderer->QueueRenderRectangle(r);
        }
        else if (render.shape == ShapeType::Ball)
        {
            RenderBall b;
            b.center = transform.position;
            b.radius = (transform.scale.x + transform.scale.y) * 0.5f * 0.5f;
            b.color = render.color;
            renderer->QueueRenderBall(b);
        }
    }
}

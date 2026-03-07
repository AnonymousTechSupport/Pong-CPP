#pragma once
#include "renderer/irenderer.h"
#include "utils/iinput.h"
#include "utils/logger.h"
#include "utils/math_utils.h"
#include <memory>
#include <vector>

// ------------------------------------------------------------------------------
// ------------------------------ COMPONENT TYPES --------------------------------
// ------------------------------------------------------------------------------

// --- Public types
// --------------
enum class ShapeType
{
    Rectangle,
    Ball
};

struct TransformComponent
{
    Vector2 position;
    Vector2 scale;
};

struct RenderComponent
{
    ShapeType shape;
    ColorUtil color;
};

struct InputComponent
{
    float speed = 360.0f;
    bool isControllable = true;
};

// ------------------

// ------------------------------------------------------------------------------
// --------------------------------- ENTITY -------------------------------------
// ------------------------------------------------------------------------------
class Entity
{
  public:
    uint16_t id = 0;

    // Components
    std::unique_ptr<TransformComponent> transform;
    std::unique_ptr<RenderComponent> render;
    std::unique_ptr<InputComponent> input;
};

// ------------------------------------------------------------------------------
// ------------------------------ ENTITY MANAGER -------------------------------
// ------------------------------------------------------------------------------
class EntityManager
{
  private:
    std::vector<std::unique_ptr<Entity>> m_entities;
    uint16_t m_nextEntityId = 0;

  public:
    // Creation / destruction
    Entity* CreateEntity()
    {
        auto entity = std::make_unique<Entity>();
        entity->id = m_nextEntityId++;
        m_entities.push_back(std::move(entity));
        return m_entities.back().get();
    }

    // ------------------------------------------------------------------------------
    // ----------------------- COMPONENT ACCESSORS (ADD/GET/REMOVE) ------------------
    // ------------------------------------------------------------------------------
    void AddTransform(Entity* e, const TransformComponent& t)
    {
        if (!e)
            return;
        e->transform = std::make_unique<TransformComponent>(t);
    }

    TransformComponent* GetTransform(Entity* e)
    {
        if (!e || !e->transform)
            return nullptr;
        return e->transform.get();
    }

    void RemoveTransform(Entity* e)
    {
        if (!e)
            return;
        e->transform.reset();
    }

    void AddRender(Entity* e, const RenderComponent& r)
    {
        if (!e)
            return;
        e->render = std::make_unique<RenderComponent>(r);
    }

    RenderComponent* GetRender(Entity* e)
    {
        if (!e || !e->render)
            return nullptr;
        return e->render.get();
    }

    void RemoveRender(Entity* e)
    {
        if (!e)
            return;
        e->render.reset();
    }

    void AddInput(Entity* e, const InputComponent& i)
    {
        if (!e)
            return;
        e->input = std::make_unique<InputComponent>(i);
    }

    InputComponent* GetInput(Entity* e)
    {
        if (!e || !e->input)
            return nullptr;
        return e->input.get();
    }

    void RemoveInput(Entity* e)
    {
        if (!e)
            return;
        e->input.reset();
    }

    // ------------------------------------------------------------------------------
    // ---------------------------- SYSTEMS / HELPERS -------------------------------
    // ------------------------------------------------------------------------------
    void UpdateAll(float dt)
    {
        for (auto& entity : m_entities)
        {
            if (entity->input && entity->transform)
            {
                auto entityTransform = entity->transform.get();
                auto entityInput = entity->input.get();

                // Movement uses simple ASCII letter checks: 'W'/'A'/'S'/'D'.
                // Input resolves these to scancodes internally so game code stays SDL-free.
                if (input::IsKeyDown('W'))
                    entityTransform->position.y -= entityInput->speed * dt;
                if (input::IsKeyDown('S'))
                    entityTransform->position.y += entityInput->speed * dt;
                if (input::IsKeyDown('A'))
                    entityTransform->position.x -= entityInput->speed * dt;
                if (input::IsKeyDown('D'))
                    entityTransform->position.x += entityInput->speed * dt;
            }
        }
    }

    // ------------------------------------------------------------------------------
    // -------------------------- RENDER HELPERS (QUEUE COMMANDS) -------------------
    // ------------------------------------------------------------------------------
    void DrawAll(IRenderer* renderer)
    {
        if (!renderer)
        {
            LOG_ERROR("DrawAll called with null renderer");
            return;
        }

        for (const auto& entity : m_entities)
        {
            if (entity->render && entity->transform)
            {
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
                    // interpret scale.x as diameter
                    b.radius = (transform.scale.x + transform.scale.y) * 0.5f * 0.5f;
                    b.color = render.color;
                    renderer->QueueRenderBall(b);
                }
            }
        }
    }
};

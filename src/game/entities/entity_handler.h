#pragma once
#include "renderer/interface/irenderer.h"
#include "utils/math/math_utils.h"
#include <memory>
#include <vector>

// --- Component types ---

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

// --- Entity ---

class Entity
{
  public:
    uint16_t id = 0;

    // Components
    std::unique_ptr<TransformComponent> transform;
    std::unique_ptr<RenderComponent> render;
    std::unique_ptr<InputComponent> input;
};

// --- EntityManager ---

class EntityManager
{
  private:
    std::vector<std::unique_ptr<Entity>> m_entities;
    uint16_t m_nextEntityId = 0;

  public:
    // Creation
    Entity* CreateEntity()
    {
        auto entity = std::make_unique<Entity>();
        entity->id = m_nextEntityId++;
        m_entities.push_back(std::move(entity));
        return m_entities.back().get();
    }

    // --- Component accessors (Add / Get / Remove)

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

    // --- Systems

    // Runs input-driven movement for all entities.
    void UpdateAll(float dt);

    // Queues render commands for all entities.
    void DrawAll(IRenderer* renderer);
};

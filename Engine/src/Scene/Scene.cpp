#include "Scene.h"

#include "Components.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "Logger/Log.h"
#include "Renderer/Renderer2D.h"

namespace Apollo
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>(name);
        tag.tag = name.empty() ? "Entity" : name;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::OnRuntimeStart()
    {
        b2Vec2 gravity = {0.0f, -100.0f };
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = gravity;
        m_physicsWorldID = b2CreateWorld(&worldDef);

        auto view = m_registry.view<RigidBody2DComponent>();
        for (auto e : view)
        {
            Entity entity = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = rb2d.type;
            bodyDef.position = {transform.position.x, transform.position.y};
            bodyDef.rotation = b2MakeRot(transform.rotation.z);

            b2BodyId bodyId = b2CreateBody(m_physicsWorldID, &bodyDef);
            rb2d.bodyId = bodyId;

            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

                b2Polygon box = b2MakeBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                shapeDef.density = bc2d.density;
                shapeDef.material.friction = bc2d.friction;
                shapeDef.material.restitution = bc2d.restitution;
                b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
                bc2d.shapeId = shapeId;
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        b2DestroyWorld(m_physicsWorldID);
        m_physicsWorldID = b2_nullWorldId;
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
        }

        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(Timestep ts)
    {
        // Update scripts
        {
            m_registry.view<NativeScriptComponent>().each([=, this](auto entity, auto& nsc)
            {
                if (!nsc.instance)
                {
                    nsc.instance = nsc.InstantiateScript();
                    nsc.instance->m_entity = Entity{ entity, this };

                    nsc.instance->OnCreate();
                }

                 nsc.instance->OnUpdate(ts);
            });
        }

        // Physics
        {
            constexpr int subStepCount = 1;
            b2World_Step(m_physicsWorldID, ts, subStepCount);

            auto view = m_registry.view<RigidBody2DComponent>();
            for (auto e : view)
            {
                Entity entity = {e, this};
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                b2Vec2 position = b2Body_GetPosition(rb2d.bodyId);
                b2Rot rotation = b2Body_GetRotation(rb2d.bodyId);
                transform.position.x = position.x;
                transform.position.y = position.y;
                transform.rotation.z = b2Rot_GetAngle(rotation);
            }
        }

        // Render 2D
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.primary)
                {
                    mainCamera = &camera.camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
            }

            Renderer2D::EndScene();
        }

    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;

        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio)
            {
                if (m_viewportWidth > 0 && m_viewportHeight > 0)
                    cameraComponent.camera.SetViewportSize(width, height);
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.primary)
                return Entity {entity, this};
        }
        return {};
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }


    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if (m_viewportWidth > 0 && m_viewportHeight > 0)
            component.camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {

    }
}

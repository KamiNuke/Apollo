#pragma once

#include <entt/entt.hpp>
#include <box2d/id.h>

#include "Core/Timestep.h"
#include "Renderer/EditorCamera.h"

namespace Apollo
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnUpdateEditor(Timestep ts, EditorCamera& camera);
        void OnUpdateRuntime(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_registry;
        uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

        b2WorldId m_physicsWorldID;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}

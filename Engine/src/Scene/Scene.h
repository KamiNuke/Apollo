#pragma once

#include <entt/entt.hpp>

#include "Core/Timestep.h"

namespace Apollo
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);
    private:
        entt::registry m_registry;
        uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

        friend class Entity;
        friend class SceneHierarchyPanel;
    };
}

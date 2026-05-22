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
    private:
        entt::registry m_registry;

        friend class Entity;
    };
}

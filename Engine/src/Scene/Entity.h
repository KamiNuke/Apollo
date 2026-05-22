#pragma once
#include "Scene.h"
#include <entt/entt.hpp>

#include "Core/Base.h"

namespace Apollo
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity&) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            APOLLO_ASSERT(!HasComponent<T>(), "Entity already has component!");

            return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            APOLLO_ASSERT(HasComponent<T>(), "Entity has no such component");

            return m_scene->m_registry.get<T>(m_entityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_scene->m_registry.any_of<T>(m_entityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            APOLLO_ASSERT(HasComponent<T>(), "Entity has no such component");

            m_scene->m_registry.remove<T>(m_entityHandle);
        }

        operator bool() const { return m_entityHandle != entt::null; }

        operator uint32_t() const { return (uint32_t)m_entityHandle; }

        bool operator==(const Entity& other) const
        {
            return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }
    private:
        entt::entity m_entityHandle { entt::null };
        Scene* m_scene = nullptr;
    };
} // Apollo

#include "Entity.h"

namespace Apollo
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_entityHandle(handle), m_scene(scene)
    {
    }
} // Apollo
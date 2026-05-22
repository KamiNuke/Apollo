#pragma once
#include "Entity.h"

namespace Apollo
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        ScriptableEntity() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Timestep ts) {}

    private:
        Entity m_entity;
        friend class Scene;
    };
} // Apollo

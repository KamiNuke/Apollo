#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Core/Base.h"
#include "Renderer/Texture.h"

namespace Apollo
{
    struct TagComponent
    {
        std::string tag{};

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& _tag)
            : tag(_tag) {}
    };

    struct TransformComponent
    {
        glm::mat4 transform = { 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& _transform)
            : transform(_transform) {}

        operator glm::mat4& () { return transform; }
        operator const glm::mat4& () const { return transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> texture;
        float tilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& _color)
            : color(_color) {}
    };

    struct CameraComponent
    {
        SceneCamera camera;
        bool primary = true; // TODO: MOVE TO SCENE
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent
    {
        ScriptableEntity* instance = nullptr;

        ScriptableEntity*(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };

        }
    };
}

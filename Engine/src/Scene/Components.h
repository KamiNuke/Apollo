#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "SceneCamera.h"
#include "box2d/types.h"
#include "Core/Base.h"
#include "Core/UUID.h"

#include "Renderer/Texture.h"

namespace Apollo
{
    struct IDComponent
    {
        UUID ID;

        IDComponent() =default;
        IDComponent(const IDComponent&) = default;
        IDComponent(UUID id) { ID = id; }
    };

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
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };


        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& _position)
            : position(_position) {}

        glm::mat4 GetTransform() const
        {
            glm::mat4 transform = glm::toMat4(glm::quat(rotation));

            return glm::translate(glm::mat4(1.0f), position)
                    * transform
                    * glm::scale(glm::mat4(1.0f), scale);
        }
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

    class ScriptableEntity;
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

    struct RigidBody2DComponent
    {
        b2BodyType type = b2BodyType::b2_staticBody;
        b2BodyId bodyId;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 offset = {0.0f, 0.0f};
        glm::vec2 size = {0.5f, 0.5f};

        float density = 1.0f;
        float friction = 0.3f;
        float restitution = 0.0f;

        b2ShapeId shapeId;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };
}

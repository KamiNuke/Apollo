#include "SceneSerializer.h"

#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"
#include "Logger/Log.h"

namespace YAML
{
    template<>
        struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Apollo
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2 v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_scene(scene)
    {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << "1234";

        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = entity.GetComponent<TagComponent>().tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;

            auto& transform = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << transform.position;
            out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
            out << YAML::Key << "Scale" << YAML::Value << transform.scale;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType());
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
            out << YAML::EndMap;

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap;

            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;
            if (spriteRendererComponent.texture)
                out << YAML::Key << "Texture" << YAML::Value << spriteRendererComponent.texture->GetPath();

            out << YAML::EndMap;
        }


        if (entity.HasComponent<RigidBody2DComponent>())
        {
            out << YAML::Key << "RigidBody2DComponent";
            out << YAML::BeginMap;

            auto& rigidBody2DComponent = entity.GetComponent<RigidBody2DComponent>();
            out << YAML::Key << "BodyType" << YAML::Value << rigidBody2DComponent.type;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap;

            auto& boxCollider2DComponent = entity.GetComponent<BoxCollider2DComponent>();
            out << YAML::Key << "Offset" << YAML::Value << boxCollider2DComponent.offset;
            out << YAML::Key << "Size" << YAML::Value << boxCollider2DComponent.size;
            out << YAML::Key << "Density" << YAML::Value << boxCollider2DComponent.density;
            out << YAML::Key << "Friction" << YAML::Value << boxCollider2DComponent.friction;
            out << YAML::Key << "Restitution" << YAML::Value << boxCollider2DComponent.restitution;

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        for (auto entityID: m_scene->m_registry.view<entt::entity>())
        {
            Entity entity = {entityID, m_scene.get()};
            if (!entity)
                return;

            SerializeEntity(out, entity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        if (!std::filesystem::exists("assets/scenes"))
            std::filesystem::create_directories("assets/scenes");


        std::ofstream ofout(filepath);
        ofout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        APOLLO_ASSERT(false, "Not implemented");
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile(filepath);
        }
        catch (YAML::ParserException& e)
        {
            APOLLO_LOGGER_ERROR("Failed to load .apollo file '{0}'\n   {1}", filepath, e.what());
            return false;
        }

        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        APOLLO_LOGGER_TRACE("Dserealizing scene {0}", sceneName);

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                APOLLO_LOGGER_TRACE("Deserealized entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = m_scene->CreateEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    auto& transform = deserializedEntity.GetComponent<TransformComponent>();
                    transform.position = transformComponent["Position"].as<glm::vec3>();
                    transform.rotation = transformComponent["Rotation"].as<glm::vec3>();
                    transform.scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();

                    YAML::Node cameraProps = cameraComponent["Camera"];
                    cc.camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

                    cc.camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    cc.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cc.primary = cameraComponent["Primary"].as<bool>();
                    cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }

                YAML::Node spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& spriteComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    spriteComponent.color = spriteRendererComponent["Color"].as<glm::vec4>();
                    if (spriteRendererComponent["Texture"])
                    {
                        spriteComponent.texture = Texture2D::Create(spriteRendererComponent["Texture"].as<std::string>());
                    }
                }


                YAML::Node rigidBody2DComponent = entity["RigidBody2DComponent"];
                if (rigidBody2DComponent)
                {
                    auto& r2bd = deserializedEntity.AddComponent<RigidBody2DComponent>();
                    r2bd.type = static_cast<b2BodyType>(rigidBody2DComponent["BodyType"].as<int>());
                }

                YAML::Node boxCollider2DComponent = entity["BoxCollider2DComponent"];
                if (boxCollider2DComponent)
                {
                    auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
                    bc2d.offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
                    bc2d.size = boxCollider2DComponent["Size"].as<glm::vec2>();
                    bc2d.density = boxCollider2DComponent["Density"].as<float>();
                    bc2d.friction = boxCollider2DComponent["Friction"].as<float>();
                    bc2d.restitution = boxCollider2DComponent["Restitution"].as<float>();
                }

            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        APOLLO_ASSERT(false, "Not implemented");
        return false;
    }
} // Apollo
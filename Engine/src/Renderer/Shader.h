#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Base.h"

namespace Apollo
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetBool(const std::string& name, bool value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) const = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) const = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

        [[nodiscard]] virtual uint32_t GetID() const = 0;
        [[nodiscard]] virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

    private:
        std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
} // Apollo

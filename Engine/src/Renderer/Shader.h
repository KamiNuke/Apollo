#pragma once

#include <cstdint>
#include <string>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        virtual void SetFloat2(const std::string& name, glm::vec2 value) const = 0;
        virtual void SetFloat3(const std::string& name, glm::vec3 value) const = 0;
        virtual void SetFloat4(const std::string& name, glm::vec4 value) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

        static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
        [[nodiscard]] virtual uint32_t GetID() const = 0;
    };
} // Apollo

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
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind();
        void Unbind();

        void SetInt(const std::string& name, int value) const;
        void SetBool(const std::string& name, bool value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetMat4(const std::string& name, const glm::mat4& value) const;

        [[nodiscard]] uint32_t GetID() const { return m_programID; }
    private:
        uint32_t m_programID;
    };
} // Apollo

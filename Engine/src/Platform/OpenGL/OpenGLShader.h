#pragma once
#include "Renderer/Shader.h"

namespace Apollo
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        void SetInt(const std::string& name, int value) const override;
        void SetBool(const std::string& name, bool value) const override;
        void SetFloat(const std::string& name, float value) const override;
        void SetFloat2(const std::string& name, glm::vec2 value) const override;
        void SetFloat3(const std::string& name, glm::vec3 value) const override;
        void SetFloat4(const std::string& name, glm::vec4 value) const override;
        void SetMat4(const std::string& name, const glm::mat4& value) const override;

        [[nodiscard]] uint32_t GetID() const override { return m_programID; }

    private:
        uint32_t m_programID;
    };
} // Apollo

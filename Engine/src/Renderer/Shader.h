#pragma once

#include <cstdint>
#include <string>

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
    private:
        uint32_t m_programID;
    };
} // Apollo

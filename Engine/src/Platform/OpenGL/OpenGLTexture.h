#pragma once
#include "glad/glad.h"
#include "Renderer/Texture.h"

namespace Apollo
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        void Bind(uint32_t slot = GL_TEXTURE0) const override;

        [[nodiscard]] uint32_t GetWidth() const override { return m_width; }
        [[nodiscard]] uint32_t GetHeight() const override { return m_height; }
        [[nodiscard]] const std::string& GetPath() const override { return m_path; }
        [[nodiscard]] uint32_t GetID() const override { return m_textureID; }

    private:
        std::string m_path;
        uint32_t m_width, m_height;
        uint32_t m_textureID;
    };
} // Apollo

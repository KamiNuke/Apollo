#include "OpenGLTexture.h"

#include <cassert>

#include "glad/glad.h"
#include "stb_image.h"
#include "Logger/Log.h"

namespace Apollo
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_path(path)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        assert(data && "failed to load texture");
        m_width = width;
        m_height = height;

        GLenum internalFormat = 0, format = 0;
        if (nrChannels == 4)
        {
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
        }
        else if (nrChannels == 3)
        {
            internalFormat = GL_RGB8;
            format = GL_RGB;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_textureID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
} // Apollo
#include "glad/glad.h"
#include "OpenGLFramebuffer.h"

#include "Logger/Log.h"

namespace Apollo
{
    static constexpr uint32_t s_maxFramebufferSize = 8192;

    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void GenTexture(uint32_t* outID, uint32_t count)
        {
            glGenTextures(count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum internalFormat, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::DEPTH_24_STENCIL_8: return true;
                default: break;
            }
            return false;
        }
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
        : m_specification(spec)
    {
        for (auto specification : m_specification.attachments.attachments)
        {
            if (!Utils::IsDepthFormat(specification.textureFormat))
                m_colorAttachmentSpecifications.emplace_back(specification);
            else
                m_depthAttachmentSpecification = specification;

        }

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_frameBuffer)
        {
            glDeleteFramebuffers(1, &m_frameBuffer);
            glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
            glDeleteTextures(1, &m_depthAttachment);

            m_colorAttachments.clear();
            m_depthAttachment = 0;
        }

        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        bool multisample = m_specification.samples > 1;

        // Attachments
        if (m_colorAttachmentSpecifications.size())
        {
            m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
            Utils::GenTexture(m_colorAttachments.data(), m_colorAttachments.size());

            for (size_t i = 0; i < m_colorAttachments.size(); i++)
            {
                Utils::BindTexture(multisample, m_colorAttachments[i]);
                switch (m_colorAttachmentSpecifications[i].textureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_RGBA8, GL_RGBA,
                            m_specification.width, m_specification.height,i);
                        break;

                    case FramebufferTextureFormat::RED_INTEGER:
                        Utils::AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_R32I, GL_RED_INTEGER,
                            m_specification.width, m_specification.height,i);
                        break;

                    default: break;
                }
            }
        }

        if (m_depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
        {
            Utils::GenTexture(&m_depthAttachment, 1);
            Utils::BindTexture(multisample, m_depthAttachment);
            switch (m_depthAttachmentSpecification.textureFormat)
            {
                case FramebufferTextureFormat::DEPTH_24_STENCIL_8:
                    Utils::AttachDepthTexture(m_depthAttachment, m_specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                        m_specification.width, m_specification.height);
                    break;
                case FramebufferTextureFormat::None:
                    break;
                case FramebufferTextureFormat::RGBA8:
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    break;
                default:
                    break;
            }
        }

        if (m_colorAttachments.size() > 1)
        {
            APOLLO_ASSERT(m_colorAttachments.size() <= 4, "");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_colorAttachments.size(), buffers);
        }
        else if (m_colorAttachments.empty())
        {
            glDrawBuffer(GL_NONE);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            APOLLO_LOGGER_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glViewport(0, 0, m_specification.width, m_specification.height);
    }

    void OpenGLFramebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (m_specification.width == width && m_specification.height == height ||
            width == 0 || height == 0 ||
            width > s_maxFramebufferSize || height > s_maxFramebufferSize)
        {
            APOLLO_LOGGER_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }


        m_specification.width = width;
        m_specification.height = height;

        Invalidate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        APOLLO_ASSERT(attachmentIndex < m_colorAttachments.size(), "");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        APOLLO_ASSERT(attachmentIndex < m_colorAttachments.size(), "");

        glClearBufferiv(GL_COLOR, attachmentIndex, &value);
    }
} // Apollo
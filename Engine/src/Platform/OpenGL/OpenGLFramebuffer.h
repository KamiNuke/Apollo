#pragma once
#include "Renderer/Framebuffer.h"

namespace Apollo
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer() override;

        void Invalidate();

        void Bind() const override;
        void Unbind() const override;

        void Resize(uint32_t width, uint32_t height) override;

        [[nodiscard]] uint32_t GetColorAttachmentRendererID() const override { return m_textureColorBuffer; }
        [[nodiscard]] const FramebufferSpecification& GetSpecification() const override { return m_specification; }
    private:
        uint32_t m_frameBuffer, m_depthBuffer;
        uint32_t m_textureColorBuffer;
        FramebufferSpecification m_specification;
    };
} // Apollo

#pragma once
#include <cassert>

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
        int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        void ClearAttachment(uint32_t attachmentIndex, int value) override;

        [[nodiscard]] uint32_t GetColorAttachmentRendererID(uint32_t index) const override { APOLLO_ASSERT(index < m_colorAttachments.size(), "GetColorAttachmentRendererID index is bigger than array"); return m_colorAttachments[index]; }
        [[nodiscard]] const FramebufferSpecification& GetSpecification() const override { return m_specification; }
    private:
        uint32_t m_frameBuffer;
        FramebufferSpecification m_specification;

        std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
        FramebufferTextureSpecification m_depthAttachmentSpecification = FramebufferTextureFormat::None;

        std::vector<uint32_t> m_colorAttachments;
        uint32_t m_depthAttachment = 0;
    };
} // Apollo

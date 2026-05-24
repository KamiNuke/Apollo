#pragma once
#include <cstdint>
#include <vector>

#include "Core/Base.h"

namespace Apollo
{
    enum class FramebufferTextureFormat
    {
        None = 0,

        // Color
        RGBA8,
        RED_INTEGER,

        // Depth/stencil
        DEPTH_24_STENCIL_8,

        // Defaults
        Depth = DEPTH_24_STENCIL_8
    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(const FramebufferTextureFormat format)
            : textureFormat(format) {}

        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments)
            : attachments(_attachments) {}

        std::vector<FramebufferTextureSpecification> attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t width = 0, height = 0;
        FramebufferAttachmentSpecification attachments;
        uint32_t samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

        [[nodiscard]] virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
        [[nodiscard]] virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
} // Apollo

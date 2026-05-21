#pragma once
#include <cstdint>

#include "Core/Base.h"

namespace Apollo
{
    struct FramebufferSpecification
    {
        uint32_t width{}, height{};
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
        [[nodiscard]] virtual uint32_t GetColorAttachmentRendererID() const = 0;
        [[nodiscard]] virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
} // Apollo

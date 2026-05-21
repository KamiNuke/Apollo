#include "Framebuffer.h"

#include "Core/Base.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Apollo
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLFramebuffer>(spec);
            case RendererAPI::API::Vulkan:
                assert("RendererAPI::Vulkan is not supported!");
                return nullptr;
            default:
                break;
        }

        assert("UNKNOWN RendererAPI!");
        return nullptr;
    }
} // Apollo
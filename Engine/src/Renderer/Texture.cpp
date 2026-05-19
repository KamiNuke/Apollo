#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Apollo
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
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
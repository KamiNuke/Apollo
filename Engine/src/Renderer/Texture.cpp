#include "Texture.h"

#include "Renderer.h"
#include "Core/ResourceManager.h"
#include "Logger/Log.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Apollo
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height, data, size);
            case RendererAPI::API::Vulkan:
                assert("RendererAPI::Vulkan is not supported!");
                return nullptr;
            default:
                break;
        }

        assert("UNKNOWN RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        Ref<Texture2D> texture = ResourceManager::Get().GetTexture2D(path);
        if (texture != nullptr)
            return texture;
        APOLLO_LOGGER_INFO("Loading a texture: {0}", path);

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
            {
                Ref<Texture2D> tex2D = CreateRef<OpenGLTexture2D>(path);
                ResourceManager::Get().AddTexture2D(path, tex2D);
                return tex2D;
            }
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
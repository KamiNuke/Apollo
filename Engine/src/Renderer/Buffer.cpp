#include "Buffer.h"

#include <cassert>

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/Renderer.h"

namespace Apollo
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
            case RendererAPI::API::Vulkan:
                assert("RendererAPI::Vulkan is not supported!");
                return nullptr;
            default:
                break;
        }

        assert("UNKNOWN RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
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
#include "VertexArray.h"

#include <cassert>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Apollo
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                assert("RendererAPI::None is not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
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
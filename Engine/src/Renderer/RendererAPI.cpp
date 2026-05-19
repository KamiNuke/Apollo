#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Apollo
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
            case RendererAPI::API::None: assert("RendererAPI::None is not supported"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
            case RendererAPI::API::Vulkan: assert("RendererAPI::Vulkan is not supported"); return nullptr;
            default: break;
        }

        assert("Unknown RendererAPI");
        return nullptr;
    }
} // Apollo
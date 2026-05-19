#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Apollo
{
    Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::Create();

} // Apollo
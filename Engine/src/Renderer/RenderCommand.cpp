#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Apollo
{
    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;

} // Apollo
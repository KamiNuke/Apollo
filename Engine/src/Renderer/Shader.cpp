#include "Shader.h"

#include <cassert>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Apollo
{
	Scope<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				assert("RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLShader>(vertexSource, fragmentSource);
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
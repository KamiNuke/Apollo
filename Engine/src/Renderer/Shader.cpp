#include "Shader.h"

#include <cassert>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Apollo
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				assert("RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLShader>(filepath);
			case RendererAPI::API::Vulkan:
				assert("RendererAPI::Vulkan is not supported!");
				return nullptr;
			default:
				break;
		}

		assert("UNKNOWN RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				assert("RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLShader>(name, vertexSource, fragmentSource);
			case RendererAPI::API::Vulkan:
				assert("RendererAPI::Vulkan is not supported!");
				return nullptr;
			default:
				break;
		}

		assert("UNKNOWN RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		assert(m_shaders.contains(name) && "Shader already exists");
		m_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		assert(m_shaders.contains(name) && "Shader not found!");
		return m_shaders[name];
	}
} // Apollo
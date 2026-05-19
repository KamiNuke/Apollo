#include "glad/glad.h"
#include "OpenGLShader.h"

#include "Logger/Log.h"

namespace Apollo
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		assert("Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_programID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_programID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::SetBool(const std::string& name, bool value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1i(location, static_cast<int>(value));
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, glm::vec2 value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const std::string& name, glm::vec4 value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const
    {
        const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			APOLLO_LOGGER_ERROR("Couldn't open file {0}", filepath);
		}

		return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		const size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			assert(ShaderTypeFromString(type) && "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));		}

		return shaderSources;
    }

    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
    {
		m_programID = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar *sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				APOLLO_LOGGER_INFO("{0}", infoLog.data());
				break;
			}

			glAttachShader(m_programID, shader);
			glShaderIDs.push_back(shader);
		}

		// Link our program
		glLinkProgram(m_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_programID);
			// Don't leak shaders either.

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			APOLLO_LOGGER_INFO("{0}", infoLog.data());
			assert("Shader linking failed");
			return;
		}

			for (auto id : glShaderIDs)
				glDetachShader(m_programID, id);
    }
} // Apollo
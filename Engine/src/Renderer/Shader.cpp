#include "Shader.h"
#include "glad/glad.h"
#include <fstream>

#include "Logger/Log.h"

namespace Apollo
{
    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			APOLLO_LOGGER_INFO("{0}", infoLog.data());
			assert("Vertex shader compilation failed");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			APOLLO_LOGGER_INFO("{0}", infoLog.data());
			assert("Fragment shader compilation failed");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_programID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);

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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			APOLLO_LOGGER_INFO("{0}", infoLog.data());
			assert("Shader linking failed");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_programID, vertexShader);
		glDetachShader(m_programID, fragmentShader);
    }

    Shader::~Shader()
    {
    	glDeleteProgram(m_programID);
    }

    void Shader::Bind()
    {
        glUseProgram(m_programID);
    }

    void Shader::Unbind()
    {
    	glUseProgram(0);
    }

    void Shader::SetInt(const std::string& name, int value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::SetBool(const std::string& name, bool value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1i(location, static_cast<int>(value));
    }

    void Shader::SetFloat(const std::string& name, float value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetFloat2(const std::string& name, glm::vec2 value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
    	glUniform2f(location, value.x, value.y);
    }

    void Shader::SetFloat3(const std::string& name, glm::vec3 value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
    	glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::SetFloat4(const std::string& name, glm::vec4 value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
    	glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
    {
    	const GLint location = glGetUniformLocation(m_programID, name.c_str());
    	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
} // Apollo
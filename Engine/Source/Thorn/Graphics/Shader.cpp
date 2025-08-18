#include "Thorn/pch.h"
#include "Shader.h"

#include "Thorn/Lib/File.h"
#include "glad/glad.h"

namespace Thorn
{
	Shader::Shader(const ShaderSpecs& specs)
		: m_Specs(specs), m_ProgramID(0)
	{
		std::string vertSrc = File(specs.VertexPath).ReadString();
		std::string fragSrc = File(specs.FragmentPath).ReadString();

        const char* vertCode = vertSrc.c_str();
        const char* fragCode = fragSrc.c_str();

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertCode, NULL);
        glCompileShader(vertexShader);

        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            Log::Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
        }

        // fragment shader
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragCode, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            Log::Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
        }

        // link shaders
        m_ProgramID = glCreateProgram();
        glAttachShader(m_ProgramID, vertexShader);
        glAttachShader(m_ProgramID, fragmentShader);
        glLinkProgram(m_ProgramID);
        // check for linking errors
        glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
            Log::Error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
	}

    void Shader::Use() const
    {
        glUseProgram(m_ProgramID);
    }
}
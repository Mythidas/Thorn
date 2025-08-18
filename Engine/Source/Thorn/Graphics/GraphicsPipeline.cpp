#include "Thorn/pch.h"
#include "GraphicsPipeline.h"
#include "Thorn/Lib/File.h"

#include <glad/glad.h>

namespace Thorn
{
	namespace Utils
	{
		static bool LogShaderError(uint32_t shader)
		{
			int success;
			char infoLog[512];

			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, nullptr, infoLog);
				Log::Error("Shader Compile Error");
				return false;
			}

			return true;
		}

		static bool LogProgramError(uint32_t program)
		{
			int success;
			char infoLog[512];

			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(program, 512, nullptr, infoLog);
				Log::Error("Program Link Error");
				return false;
			}

			return true;
		}

		static int GetAttributeSize(VertexAttribute attrib)
		{
			switch (attrib)
			{
			case VertexAttribute::Float: return 4 * 1;
			case VertexAttribute::Float2: return 4 * 2;
			case VertexAttribute::Float3: return 4 * 3;
			case VertexAttribute::Float4: return 4 * 4;
			default: return 4;
			}
		}

		static int GetAttributeCount(VertexAttribute attrib)
		{
			switch (attrib)
			{
			case VertexAttribute::Float: return 1;
			case VertexAttribute::Float2: return 2;
			case VertexAttribute::Float3: return 3;
			case VertexAttribute::Float4: return 4;
			default: return 1;
			}
		}

		static GLenum GetAttributeType(VertexAttribute attrib)
		{
			switch (attrib)
			{
			case VertexAttribute::Float: return GL_FLOAT;
			case VertexAttribute::Float2: return GL_FLOAT;
			case VertexAttribute::Float3: return GL_FLOAT;
			case VertexAttribute::Float4: return GL_FLOAT;
			default: return GL_FLOAT;
			}
		}
	}

	GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSpecs& specs)
		: m_Specs(specs), m_RenderID(0), m_VertexID(0)
	{
		std::string vertSrc = File(specs.VertPath).ReadString();
		std::string fragSrc = File(specs.FragPath).ReadString();

		const char* vertCode = vertSrc.c_str();
		const char* fragCode = fragSrc.c_str();

		uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vertCode, nullptr);
		glCompileShader(vertShader);

		if (!Utils::LogShaderError(vertShader))
		{
			Log::Error("Failed to Create Shader");
			return;
		}

		uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragCode, nullptr);
		glCompileShader(fragShader);

		if (!Utils::LogShaderError(fragShader))
		{
			Log::Error("Failed to Create Shader");
			return;
		}

		m_RenderID = glCreateProgram();
		glAttachShader(m_RenderID, vertShader);
		glAttachShader(m_RenderID, fragShader);
		glLinkProgram(m_RenderID);

		if (!Utils::LogProgramError(m_RenderID))
		{
			Log::Error("Failed to Create Program ID");
			return;
		}

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		glGenVertexArrays(1, &m_VertexID);
		glBindVertexArray(m_VertexID);

		m_Specs.pVertexBuffer->Bind();
		m_Specs.pIndexBuffer->Bind();

		GLsizei stride = 0;
		for (auto& attrib : m_Specs.Attributes)
		{
			stride += Utils::GetAttributeSize(attrib);
		}

		size_t offset = 0;
		for (GLuint i = 0; i < m_Specs.Attributes.size(); i++)
		{
			glEnableVertexArrayAttrib(m_VertexID, i);
			glVertexAttribPointer(i, Utils::GetAttributeCount(m_Specs.Attributes[i]), Utils::GetAttributeType(m_Specs.Attributes[i]), GL_FALSE, stride, (void*)offset);
			offset += Utils::GetAttributeSize(m_Specs.Attributes[i]);
		}

		uint32_t camera_index = glGetUniformBlockIndex(m_RenderID, "Camera");
		glUniformBlockBinding(m_RenderID, camera_index, 0);
	}

	void GraphicsPipeline::Draw(uint32_t count) const
	{
		glUseProgram(m_RenderID);
		glBindVertexArray(m_VertexID);

		for (auto& buffer : m_Specs.pRenderBuffers)
		{
			buffer->Bind();
		}

		if (m_Specs.pIndexBuffer)
		{
			m_Specs.pIndexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, count);
		}
	}
}
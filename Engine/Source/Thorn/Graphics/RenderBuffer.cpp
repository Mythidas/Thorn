#include "Thorn/pch.h"
#include "RenderBuffer.h"

#include <glad/glad.h>

namespace Thorn
{
	namespace Utils
	{
		static GLenum GetBufferUsage(BufferUsage usage)
		{
			switch (usage)
			{
			case BufferUsage::Vertex: return GL_ARRAY_BUFFER;
			case BufferUsage::Index: return GL_ELEMENT_ARRAY_BUFFER;
			case BufferUsage::Uniform: return GL_UNIFORM_BUFFER;
			case BufferUsage::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
			default: return 0;
			}
		}

		static GLenum GetDrawRate(BufferRate rate)
		{
			switch (rate)
			{
			case BufferRate::Static: return GL_STATIC_DRAW;
			case BufferRate::Dynamic: return GL_DYNAMIC_DRAW;
			default: return 0;
			}
		}

		static GLenum GetBufferAccess(BufferAccess access)
		{
			switch (access)
			{
			case BufferAccess::ReadOnly: return GL_READ_ONLY;
			case BufferAccess::WriteOnly: return GL_WRITE_ONLY;
			case BufferAccess::ReadWrite: return GL_READ_WRITE;
			default: return GL_READ_WRITE;
			}
		}
	}

	RenderBuffer::RenderBuffer(const RenderBufferSpecs& specs)
		: m_Specs(specs), m_RenderID(0)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(Utils::GetBufferUsage(specs.Usage), m_RenderID);
		glBufferData(Utils::GetBufferUsage(specs.Usage), specs.Size, nullptr, Utils::GetDrawRate(specs.Rate));

		if (specs.Usage == BufferUsage::Uniform)
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, specs.Binding, m_RenderID, 0, specs.Size);
		}
	}

	void RenderBuffer::Bind() const
	{
		glBindBuffer(Utils::GetBufferUsage(m_Specs.Usage), m_RenderID);
	}

	void RenderBuffer::SetData(void* data, size_t size, uint32_t offset)
	{
		if (size + offset > m_Specs.Size)
		{
			Log::Error("Size exceeds allocated RenderBuffer");
		}

		Bind();
		glNamedBufferSubData(m_RenderID, offset, size, data);
	}

	void* RenderBuffer::MapBufferPtr(BufferAccess access)
	{
		return glMapNamedBuffer(m_RenderID, Utils::GetBufferAccess(access));
	}

	void RenderBuffer::UnMapBufferPtr()
	{
		glUnmapNamedBuffer(m_RenderID);
	}
}
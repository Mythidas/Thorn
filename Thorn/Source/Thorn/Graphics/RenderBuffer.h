#pragma once

#include "Thorn/Lib/Pointers.h"

namespace Thorn
{
	enum class BufferUsage
	{
		Vertex,
		Index,
		Uniform,
		ShaderStorage
	};

	enum class BufferRate
	{
		Static,
		Dynamic
	};

	enum class BufferAccess
	{
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	struct RenderBufferSpecs
	{
		size_t Size;
		uint32_t Binding;
		BufferUsage Usage;
		BufferRate Rate;
	};

	class RenderBuffer
	{
	public:
		THORN_MAKE_REF(RenderBuffer)

		void Bind() const;
		void SetData(void* data, size_t size, uint32_t offset);
		void* MapBufferPtr(BufferAccess access);
		void UnMapBufferPtr();

	private:
		RenderBuffer(const RenderBufferSpecs& specs);

		RenderBufferSpecs m_Specs;
		uint32_t m_RenderID;
	};
}
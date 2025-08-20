#pragma once

#include <vector>
#include <string>

#include "Thorn/Lib/Pointers.h"
#include "RenderBuffer.h"

namespace Thorn
{
	enum class VertexAttribute
	{
		Float,
		Float2,
		Float3,
		Float4
	};

	struct ShaderSpecs
	{
		std::string VertPath;
		std::string FragPath;
		Ref<RenderBuffer> pVertexBuffer;
		Ref<RenderBuffer> pIndexBuffer;
		std::vector<Ref<RenderBuffer>> pRenderBuffers;
		std::vector<VertexAttribute> Attributes;
	};

	class Shader
	{
	public:
		THORN_MAKE_SCOPE(Shader)

		void Draw(uint32_t count) const;

	private:
		Shader(const ShaderSpecs& specs);

		ShaderSpecs m_Specs;
		uint32_t m_RenderID;
		uint32_t m_VertexID;
	};
}
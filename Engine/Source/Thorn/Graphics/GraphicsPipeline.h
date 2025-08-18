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

	struct GraphicsPipelineSpecs
	{
		std::string VertPath;
		std::string FragPath;
		Ref<RenderBuffer> pVertexBuffer;
		Ref<RenderBuffer> pIndexBuffer;
		std::vector<Ref<RenderBuffer>> pRenderBuffers;
		std::vector<VertexAttribute> Attributes;
	};

	class GraphicsPipeline
	{
	public:
		THORN_MAKE_SCOPE(GraphicsPipeline)

		void Draw(uint32_t count) const;

	private:
		GraphicsPipeline(const GraphicsPipelineSpecs& specs);

		GraphicsPipelineSpecs m_Specs;
		uint32_t m_RenderID;
		uint32_t m_VertexID;
	};
}
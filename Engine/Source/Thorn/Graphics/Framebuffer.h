#pragma once

#include "Thorn/Lib/Pointers.h"
#include "Thorn/Math/Vector.h"

namespace Thorn
{
	struct FramebufferSpecs
	{
		UVector2 Size;
	};

	class Framebuffer
	{
	public:
		THORN_MAKE_REF(Framebuffer)
		~Framebuffer();

		void Bind() const;
		void Unbind() const;
		void Resize(const UVector2& size);

		inline void* GetRenderID() const { return reinterpret_cast<void*>((uint32_t)m_Texture); }
		inline const FramebufferSpecs& GetSpecs() const { return m_Specs; }

	private:
		Framebuffer(const FramebufferSpecs& builder);

		FramebufferSpecs m_Specs;
		uint32_t m_RenderID{ 0 };
		uint32_t m_Texture{ 0 };
		uint32_t m_RenderObject{ 0 };
	};
}
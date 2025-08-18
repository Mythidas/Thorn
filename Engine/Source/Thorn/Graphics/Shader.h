#pragma once

#include "Thorn/Lib/Pointers.h"

#include <string>

namespace Thorn
{
	struct ShaderSpecs
	{
		std::string VertexPath;
		std::string FragmentPath;
	};

	class Shader
	{
	public:
		THORN_MAKE_REF(Shader)
		void Use() const;

	private:
		Shader(const ShaderSpecs& specs);

		ShaderSpecs m_Specs;
		uint32_t m_ProgramID;
	};
}
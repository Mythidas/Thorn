#pragma once

#include "Thorn/Math/Vector.h"

namespace Thorn
{
	class RenderCommands
	{
	public:
		RenderCommands();

		static void ClearColor(const Vector4& color);
		static void Resize(const UVector2& size);
		static void EnableDepthTesting(bool enable);
	};
}
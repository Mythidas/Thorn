#include "Thorn/pch.h"
#include "RenderCommands.h"

#include <glad/glad.h>

namespace Thorn
{
	RenderCommands::RenderCommands()
	{
	}

	void RenderCommands::ClearColor(const Vector4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommands::Resize(const UVector2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void RenderCommands::EnableDepthTesting(bool enable)
	{
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
}
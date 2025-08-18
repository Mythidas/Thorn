#pragma once

#include "Image.h"
#include "Color.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "Thorn/Math/Vector.h"
#include "Thorn/Math/Matrix.h"
#include "Thorn/Lib/Pointers.h"

namespace Thorn
{
	class Renderer
	{
	public:
		static void BeginFrame(const Camera& camera, const Transform& transform, bool swapBuffer = false);
		static void EndFrame();

		static void DrawQuad(const Vector3& position, const Vector3& rotation, const Vector3& scale, const Color& color);
		static void DrawQuad(const Vector3& position, const Vector3& rotation, const Vector3& scale, const Color& color, Ref<Image> texture);
		static void DrawQuad(const Matrix4& transform, const Color& color, float textureID);
		static void DrawQuad(const Matrix4& transform, const Color& color, Vector2 texCoords[4], float textureID);

		static void Resize(const UVector2& size);

		static Ref<Framebuffer> GetFramebuffer();

	private:
		friend class Application;

	private:
		static void _Init();
		static void _Destroy();

		static void _CheckBatch();
		static void _BeginBatch();
		static void _EndBatch();
		static float _GetTextureIndex(Ref<Image> texture);
	};
}
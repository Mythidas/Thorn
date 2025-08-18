#pragma once

#include "Thorn/Math/Matrix.h"
#include "Thorn/Math/Vector.h"

namespace Thorn
{
	enum class CameraMode
	{
		Perspective,
		Orthographic
	};

	struct Camera
	{
		Matrix4 Projection{ 1.0f };
		Matrix4 View{ 1.0f };
		Vector4 ClearColor{ 1.0f, 1.0f, 1.0f, 1.0f };
		CameraMode Mode{ CameraMode::Perspective };
		float FieldOfView{ 60.0f };
		float NearClip{ 0.001f };
		float FarClip{ 1000.0f };
		float OrthoSize{ 10.0f };
		bool Active{ false };

		void CalculateProjection(uint32_t width, uint32_t height);
	};
}
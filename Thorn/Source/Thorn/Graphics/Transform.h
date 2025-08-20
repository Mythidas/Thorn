#pragma once

#include "Thorn/Math/Vector.h"
#include "Thorn/Math/Matrix.h"

namespace Thorn
{
	struct Transform
	{
		Vector3 Position{ 0.0f };
		Vector3 Rotation{ 0.0f };
		Vector3 Scale{ 1.0f };

		Vector3 Forward() const;
		Vector3 Back() const;
		Vector3 Right() const;
		Vector3 Left() const;
		Vector3 Up() const;
		Vector3 Down() const;
		Matrix4 GetMatrix() const;
		Matrix4 GetInverseMatrix() const;
	};
}
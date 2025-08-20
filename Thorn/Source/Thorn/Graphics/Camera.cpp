#include "Thorn/pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Thorn
{
	Matrix4 Camera::GetProjection(const UVector2& size) const
	{
		float aspectRatio = (float)size.x / (float)size.y;

		if (Mode == CameraMode::Perspective)
			return glm::perspective(glm::radians(FieldOfView), aspectRatio, NearClip, FarClip);
		else
			return glm::ortho(-OrthoSize * aspectRatio, OrthoSize * aspectRatio, -OrthoSize, OrthoSize, NearClip, FarClip);
	}
}
#include "Thorn/pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Thorn
{
	void Camera::CalculateProjection(uint32_t width, uint32_t height)
	{
		float aspect = (float)width / (float)height;
		if (Mode == CameraMode::Perspective)
			Projection = glm::perspective(glm::radians(FieldOfView), aspect, NearClip, FarClip);
		else
			Projection = glm::ortho(-OrthoSize * aspect, OrthoSize * aspect, -OrthoSize, OrthoSize, NearClip, FarClip);
	}
}
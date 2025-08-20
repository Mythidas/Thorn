#include "Thorn/pch.h"
#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Thorn
{
	namespace Utils
	{
		static glm::quat GetOrientation(const Vector3& rotation)
		{
			return glm::quat(glm::radians(glm::vec3(-rotation.x, rotation.y, 0.0f)));
		}
	}

	Vector3 Transform::Forward() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Forward);
	}
	Vector3 Transform::Back() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Back);
	}
	Vector3 Transform::Right() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Right);
	}
	Vector3 Transform::Left() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Left);
	}
	Vector3 Transform::Up() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Up);
	}
	Vector3 Transform::Down() const
	{
		return (glm::vec3)glm::rotate(Utils::GetOrientation(Rotation), CVector3::Down);
	}
	Matrix4 Transform::GetMatrix() const
	{
		return glm::lookAt(Position, Position + Forward(), Up());
	}
	Matrix4 Transform::GetInverseMatrix() const
	{
		return glm::inverse(GetMatrix());
	}
}
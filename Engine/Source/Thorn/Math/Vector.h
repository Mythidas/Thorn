#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

using UVector2 = glm::uvec2;
using UVector3 = glm::uvec3;
using UVector4 = glm::uvec4;

using IVector2 = glm::ivec2;
using IVector3 = glm::ivec3;
using IVector4 = glm::ivec4;

struct CVector3
{
    inline static constexpr Vector3 Up = { 0.0f,  1.0f,  0.0f };
    inline static constexpr Vector3 Down = { 0.0f, -1.0f,  0.0f };
    inline static constexpr Vector3 Left = { -1.0f, 0.0f,  0.0f };
    inline static constexpr Vector3 Right = { 1.0f,  0.0f,  0.0f };
    inline static constexpr Vector3 Forward = { 0.0f,  0.0f,  1.0f };
    inline static constexpr Vector3 Back = { 0.0f,  0.0f, -1.0f };
};
#version 450

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec4 iColor;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in float iTexIndex;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec2 oTexCoord;
layout (location = 2) out float oTexIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
};

void main()
{
	gl_Position = u_Projection * u_View * vec4(iPos.x, -iPos.y, iPos.z, 1);
	oColor = iColor;
	oTexCoord = iTexCoord;
	oTexIndex = iTexIndex;
}
#version 450

layout (location = 0) in vec4 iColor;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in float iTexIndex;

layout (location = 0) out vec4 oFragColor;

layout (binding = 0) uniform sampler2D uTextures[32];

void main()
{
	int texIndex = int(iTexIndex);
	oFragColor = texture(uTextures[texIndex], iTexCoord) * iColor;
}
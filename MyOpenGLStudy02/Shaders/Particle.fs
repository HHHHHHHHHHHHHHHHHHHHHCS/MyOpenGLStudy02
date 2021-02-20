#version 460 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D sprite;
uniform vec4 particleColor;

void main()
{
	color=(texture(sprite,TexCoords)*particleColor);
}
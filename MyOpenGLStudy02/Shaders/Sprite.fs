#version 460 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
	//TODO: texture is black
	color=vec4(spriteColor,1.)*texture(image,TexCoords);
	color=texture(image,TexCoords);

}
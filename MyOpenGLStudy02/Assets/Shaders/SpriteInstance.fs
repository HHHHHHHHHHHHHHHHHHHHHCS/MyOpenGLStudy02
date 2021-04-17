#version 460 core

in vec2 TexCoords;
in float ImageIndex;

out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
	color=vec4(spriteColor,1.)*texture(image,TexCoords);
	color = vec4(ImageIndex/1000.0,ImageIndex/10000.0,0,1);
}
#version 460 core
layout(location=0)in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	TexCoords=vertex.zw;
	gl_Position=viewProjection*model*vec4(vertex.xy,0.,1.);
}

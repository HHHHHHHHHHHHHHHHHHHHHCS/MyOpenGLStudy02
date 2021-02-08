#version 460 core
layout(location=0)in vec4 vertex;//<vec2 position,vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 viewProjection;
uniform vec2 offset;
uniform vec4 color;

void main()
{
	TexCoords=vertex.zw;
	ParticleColor=color;
	gl_Position=viewProjection*model*vec4(vertex.xy,0.,1.);
}
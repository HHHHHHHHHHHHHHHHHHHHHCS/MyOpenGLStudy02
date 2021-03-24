#version 460 core
layout(location=0)in vec4 vertex;//xy pos  zw uv0
layout(location=1)in vec2 textUV;//uv1

out vec2 TexCoords0;
out vec2 TexCoords1;

uniform mat4 projection;

void main()
{
	TexCoords0=vertex.zw;
	TexCoords1=textUV;
	gl_Position=projection*vec4(vertex.xy,0.,1.);
}
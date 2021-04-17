#version 460 core
layout(location=0)in vec4 vertex;
layout(location=1)in float imageIndex;
layout(location=2)in mat4 modelMatrix;

out vec2 TexCoords;
out float ImageIndex;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	TexCoords=vertex.zw;
	ImageIndex = imageIndex;
	gl_Position=viewProjection*model*vec4(vertex.xy,0.,1.);
}

#version 460 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D _Text;
uniform vec4 _TextColor;

void main()
{
	color=vec4(_TextColor.rgb,_TextColor.a*texture(_Text,TexCoords).r);
}
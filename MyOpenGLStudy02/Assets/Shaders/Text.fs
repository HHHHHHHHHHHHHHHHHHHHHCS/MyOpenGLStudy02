#version 460 core
in vec2 TexCoords0;//做特殊效果用 暂时没有用
in vec2 TexCoords1;

out vec4 color;

uniform sampler2D _Text;
uniform vec4 _TextColor;

void main()
{
	color=vec4(_TextColor.rgb,_TextColor.a*texture(_Text,TexCoords1).r);
}
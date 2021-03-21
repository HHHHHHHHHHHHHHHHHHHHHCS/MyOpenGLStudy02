#version 460 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D _Text;
uniform vec4 _TextColor;
uniform vec4 _UV_ST;


void main()
{
	vec2 uv = TexCoords * _UV_ST.xy + _UV_ST.zw;
	color=vec4(_TextColor.rgb,_TextColor.a*texture(_Text,uv).r);
}
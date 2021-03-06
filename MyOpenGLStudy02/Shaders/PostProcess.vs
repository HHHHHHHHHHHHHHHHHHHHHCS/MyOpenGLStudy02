#version 460 core
layout(location=0)in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
	gl_Position=vec4(vertex.xy,0.,1.);
	vec2 uv=(vertex.xy+1.)/2.;

	if(chaos)
	{
		const float strength=.3;
		TexCoords=uv+strength*vec2(sin(time),cos(time));
	}
	else if(confuse)
	{
		TexCoords=1.-vec2(uv);
	}
	else
	{
		TexCoords=uv;
	}
	
	if(shake){
		float strength=.01;
		gl_Position.xy+=vec2(sin(time*10),cos(time*15))*strength;
	}
}


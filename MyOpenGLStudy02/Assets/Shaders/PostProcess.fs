#version 460 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D scene;
uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

// uniform vec2 offsets[9];
// uniform int edge_kernel[9];
// uniform float blur_kernel[9];

const float offset=1./300.;
const vec2 offsets[9]={
	{-offset,offset},// top-left
	{0.f,offset},// top-center
	{offset,offset},// top-right
	{-offset,0.f},// center-left
	{0.f,0.f},// center-center
	{offset,0.f},// center - right
	{-offset,-offset},// bottom-left
	{0.f,-offset},// bottom-center
	{offset,-offset}// bottom-right
};

const int edge_kernel[9]={
	-1,-1,-1,
	-1,8,-1,
	-1,-1,-1
};

const float blur_kernel[9]=
{
	1./16,2./16,1./16,
	2./16,4./16,2./16,
	1./16,2./16,1./16
};



void main()
{
	color=vec4(0.f,0.f,0.f,1.f);

	vec3 sampleColor[9];
	
	if(chaos||shake)
	{
		for(int i=0;i<9;i++)
		{
			sampleColor[i]=texture(scene,TexCoords.xy+offsets[i]).rgb;
		}
	}
	
	if(chaos)
	{
		for(int i=0;i<9;i++)
		{
			color.rgb+=sampleColor[i]*edge_kernel[i];
		}
	}
	else if(confuse)
	{
		color.rgb=1.-texture(scene,TexCoords).rgb;
	}
	else if(shake)
	{
		for(int i=0;i<9;i++)
		{
			color.rgb+=sampleColor[i]*blur_kernel[i];
		}
	}
	else
	{
		color=texture(scene,TexCoords);
	}
}
#pragma once
#include "Shader.h"
#include "Texture2D.h"

class PostProcessor
{
public:
	Shader postProcessingShader;
	Texture2D texture;
	GLuint width, height;
	//反色翻转轴,边缘检测,晃动
	GLboolean confuse, chaos, shake;

	PostProcessor(Shader shader, GLuint width, GLuint height);

	void BeginRender();

	void EndRender();

	void Render();
private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;

	void InitRenderData();
};

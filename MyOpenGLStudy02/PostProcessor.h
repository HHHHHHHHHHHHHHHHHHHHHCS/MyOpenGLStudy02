﻿#pragma once
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

	GLfloat shakeTimer = 0.0f;
	
	PostProcessor(Shader shader, GLuint width, GLuint height);

	void Update(GLfloat dt);
	
	void BeginRender() const;

	void EndRender() const;

	void Render(GLfloat time);

	void DoShake(GLfloat time);
	
	void Reset();

private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;

	void InitRenderData();
};

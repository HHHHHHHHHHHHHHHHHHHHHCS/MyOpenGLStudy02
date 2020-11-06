#pragma once
#include <glad/glad.h>

class Texture2D
{
public:
	static void UnBind();
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format; // 图片的Format
	GLuint Image_Format; // 加载时候图片的Format
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;

	Texture2D();
	void Generate(GLuint width, GLuint height, unsigned char* data,GLboolean mipmap = false);
	void Bind() const;
};

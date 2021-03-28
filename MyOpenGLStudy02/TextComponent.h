#pragma once
#include <string>
#include <utility>
#include <glad/glad.h>

class TextComponent
{
public:
	std::string textContext;

	TextComponent(const char* str);
	~TextComponent();
	
	void SetText(const char* str);
private:
	GLuint VAO, VBO;
};

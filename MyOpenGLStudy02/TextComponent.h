#pragma once
#include <string>
#include <vec4.hpp>
#include <glad/glad.h>

class TextRenderer;

class TextComponent
{
public:
	TextRenderer* renderer;
	std::string textContext;
	int textLength;
	float startX, startY, scale;
	glm::vec4 color;

	TextComponent();
	TextComponent(TextRenderer* textRender);
	TextComponent(TextRenderer* textRender, std::string text, float x, float y, float scale, glm::vec4 color);
	~TextComponent();

	void SetText(std::string text);
	void SetText(std::string text, float x, float y, float scale, glm::vec4 color);
	void Render();
	void DoRender() const;

private:
	GLuint VAO, VBO, EBO;
};

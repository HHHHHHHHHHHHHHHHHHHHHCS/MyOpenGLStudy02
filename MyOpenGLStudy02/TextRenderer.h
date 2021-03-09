#pragma once
#include <map>
#include <vec2.hpp>

#include "Shader.h"

struct Character
{
	unsigned int textureID;
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // offset from baseline to left/top of glyph
	unsigned int advance; // horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	std::map<char, Character> characters;
	Shader TextShader;

	TextRenderer(unsigned int width, unsigned int height);
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int VAO, VBO;
};

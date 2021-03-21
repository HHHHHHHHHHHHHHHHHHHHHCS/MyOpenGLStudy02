#pragma once
#include <map>
#include <vec2.hpp>

#include "Shader.h"

struct Character
{
	unsigned int textureID;
	unsigned int index;
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // offset from baseline to left/top of glyph
	unsigned int advance; // horizontal offset to advance to next glyph
	glm::vec4 uvST; //size and offset in texture 
};

class TextRenderer
{
public:
	std::map<char, Character> characters;
	Shader textShader;

	TextRenderer(unsigned int width, unsigned int height, Shader textShader);
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec4 color = glm::vec4(1.0f));

private:
	unsigned int VAO, VBO, textureID;
};

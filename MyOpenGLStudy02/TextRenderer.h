#pragma once
#include <map>
#include <vec2.hpp>

#include "Shader.h"
#include "TextComponent.h"

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
	std::map<std::string, TextComponent*> textComponents;

	Shader textShader;

	TextRenderer(unsigned int width, unsigned int height, Shader textShader);
	~TextRenderer();
	
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec4 color = glm::vec4(1.0f));

	TextComponent* CreateTextComponent(std::string id);
	TextComponent* CreateTextComponent(std::string id, std::string text, float x, float y, float scale,
	                                  glm::vec4 color = glm::vec4(1.0f));
	TextComponent* GetTextComponent(std::string id);
	void RenderTextComponent(std::string id);
	void RenderTextComponent(TextComponent& component);

private:
	unsigned int VAO, VBO, textureID;
};

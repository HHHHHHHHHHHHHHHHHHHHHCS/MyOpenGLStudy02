#include "TextRenderer.h"
#include "DebugLog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(unsigned width, unsigned height, Shader textShader)
{
	this->TextShader = textShader;
	textShader.Use();
	textShader.SetMatrix4x4("_Projection",
	                        glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));
	textShader.SetInteger("_Text", 0);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<const void*>(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, unsigned int fontSize)
{
	this->characters.clear();

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		DebugLog::Print("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FT_Face face;

	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		DebugLog::Print("ERROR::FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	//控制的是所读取数据的对齐方式，默认4字节对齐，
	//即一行的图像数据字节数必须是4的整数倍，即读取数据时，读取4个字节用来渲染一行
	//这里黑白 既一个像素
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c,FT_LOAD_RENDER))
		{
			DebugLog::Print("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	TextShader.Use();
	TextShader.SetVector3f("_TextColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	const float cHeight = this->characters['H'].bearing.y;

	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = characters[*c];

		float xPos = x + ch.bearing.x * scale;
		float yPos = y + (cHeight - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		float vertices[6][4] =
		{
			{xPos, yPos + h, 0.0f, 1.0f},
			{xPos + w, yPos, 1.0f, 0.0f},
			{xPos, yPos, 0.0f, 0.0f},
			{xPos, yPos + h, 0.0f, 1.0f},
			{xPos + w, yPos + h, 1.0f, 1.0f},
			{xPos + w, yPos + h, 1.0f, 0.0f},
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale; //bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

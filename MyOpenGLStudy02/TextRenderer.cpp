﻿#include "TextRenderer.h"
#include "DebugLog.h"

#include <ft2build.h>
#include <iostream>
#include <ostream>
#include <utility>


#include FT_FREETYPE_H

TextRenderer::TextRenderer(unsigned int width, unsigned int height, Shader textShader)
{
	textureID = -1;
	this->textShader = textShader;
	textShader.Use();
	textShader.SetMatrix4x4("projection",
	                        glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f));
	textShader.SetInteger("_Text", 0);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<const void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
	                      reinterpret_cast<const void*>(4 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TextRenderer::~TextRenderer()
{
	for (auto&& item : textComponents)
	{
		delete item.second;
	}
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


	const int c_count = 128;
	unsigned int maxX = 0, maxY = 0;
	unsigned int index = 0;
	for (GLubyte c = 0; c < c_count; c++, index++)
	{
		if (FT_Load_Char(face, c,FT_LOAD_RENDER))
		{
			DebugLog::Print("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		const unsigned int width = face->glyph->bitmap.width;
		const unsigned int height = face->glyph->bitmap.rows;

		if (width > maxX)
		{
			maxX = width;
		}

		if (height > maxY)
		{
			maxY = height;
		}

		Character character = {
			-1,
			index,
			glm::ivec2(width, height),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x),
			//glm::vec4(0)
		};


		glGenTextures(1, &character.textureID);
		glBindTexture(GL_TEXTURE_2D, character.textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		characters.insert(std::pair<char, Character>(c, character));
	}

	const float unit = sqrt(static_cast<float>(c_count * maxX * maxY));
	const unsigned int columnCount = static_cast<unsigned int>(ceil(unit / maxX));
	//+1 是因为 加一行char的高度  不然会少看见一行
	const unsigned int rowCount = static_cast<unsigned int>(ceil(c_count / columnCount) + 1);
	const float uv_T_X = 1.0f / columnCount;
	const float uv_T_Y = 1.0f / rowCount;
	const float uv_S_X = columnCount * maxX;
	const float uv_S_Y = rowCount * maxY;

	//text big texture

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		columnCount * maxX,
		rowCount * maxY,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		nullptr
	);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (auto&& character : characters)
	{
		const unsigned int index = character.second.index;
		const unsigned int width = character.second.size.x;
		const unsigned int height = character.second.size.y;
		const unsigned int xIndex = (index % columnCount);
		const unsigned int yIndex = static_cast<unsigned int>(floor(index / columnCount));
		const unsigned int xStartPos = maxX * xIndex;
		const unsigned int yStartPos = maxY * yIndex;

		//这个api需要gl 4.3
		glCopyImageSubData(character.second.textureID, GL_TEXTURE_2D, 0, 0, 0, 0
		                   , textureID, GL_TEXTURE_2D, 0, xStartPos, yStartPos, 0, width, height, 1);

		//这个api gl2.0 可以实现图片拷贝
		//glCopyTexSubImage2D 

		//这个api需要FBO
		//这里有对图片进行放大 , 所以下面的UV T 就是 等比缩放了  不然还要 width/(columnCount * maxX)
		// glBlitFramebuffer(0, 0, width, height, xStartPos, yStartPos, width, height,GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glDeleteTextures(1, &character.second.textureID);
		character.second.uvST = glm::vec4(static_cast<float>(width / uv_S_X), static_cast<float>(height / uv_S_Y)
		                                  , xIndex * uv_T_X, yIndex * uv_T_Y);
	}


	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);


	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec4 color)
{
	textShader.Use();
	textShader.SetVector4f("_TextColor", color);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	//我们已知的是有一些字形直接与顶部接触，如字符’H’、’T’或’X’。
	//那么我们通过接触顶部的字形的bearingY减去顶部不确定字形的bearingY来计算红色矢量的长度。
	//使用这种方法，我们依据字形顶部的点与顶部边差异的距离来向下推进字形。
	const float cHeight = static_cast<float>(this->characters['H'].bearing.y);
	const unsigned int textLength = text.length();
	GLfloat* vertices = new GLfloat[4 * 6 * textLength];
	unsigned int* indices = new unsigned int[6 * textLength];

	unsigned int index = 0;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = characters[*c];

		// textShader.SetVector4f("_UV_ST", ch.uvST);

		float xPos = x + ch.bearing.x * scale;
		float yPos = y + (cHeight - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;


		GLfloat data[4][6] =
		{
			{xPos, yPos, 0.0, 0.0, ch.uvST.z, ch.uvST.w},
			{xPos, yPos + h, 0.0, 1.0, ch.uvST.z, ch.uvST.y + ch.uvST.w},
			{xPos + w, yPos, 1.0, 0.0, ch.uvST.z + ch.uvST.x, ch.uvST.w},
			{xPos + w, yPos + h, 1.0, 1.0, ch.uvST.z + ch.uvST.x, ch.uvST.w + ch.uvST.y},
		};

		const unsigned int vertexOffset = 4 * index;
		const unsigned int indicesOffset = 6 * index;

		indices[indicesOffset + 0] = vertexOffset;
		indices[indicesOffset + 5] = vertexOffset + 3;
		indices[indicesOffset + 2] = indices[indicesOffset + 3] = vertexOffset + 2;
		indices[indicesOffset + 1] = indices[indicesOffset + 4] = vertexOffset + 1;


		memcpy(vertices + (index * 4 * 6), data, sizeof(float) * 4 * 6);

		x += (ch.advance >> 6) * scale; //bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
		index++;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6 * textLength, vertices, GL_STATIC_DRAW);
	// glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * textLength * 6 * 4, vertices);
	// glDrawArrays(GL_TRIANGLES, 0, 6 * textLength);


	glDrawElements(GL_TRIANGLES, 2 * 3 * textLength, GL_UNSIGNED_INT, indices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	delete[] vertices;
	delete[] indices;
}

TextComponent* TextRenderer::CreateTextComponent(const std::string id)
{
	delete GetTextComponent(id);
	TextComponent* tc = new TextComponent(this);
	textComponents[id] = tc;
	return tc;
}


TextComponent* TextRenderer::CreateTextComponent(std::string id, std::string text, float x, float y, float scale,
                                                 glm::vec4 color)
{
	delete GetTextComponent(id);
	TextComponent* tc = new TextComponent(this, std::move(text), x, y, scale, color);
	textComponents[id] = tc;
	return tc;
}

TextComponent* TextRenderer::GetTextComponent(const std::string id)
{
	auto it = textComponents.find(id);
	if (it == textComponents.end())
	{
		// std::cout << "Find TextComponent Error! ID: " + id << std::endl;
		return nullptr;
	}
	return it->second;
}

void TextRenderer::RenderTextComponent(const std::string id)
{
	auto it = textComponents.find(id);
	if (it != textComponents.end())
	{
		it->second->Render();
	}
}


void TextRenderer::RenderTextComponent(TextComponent& component)
{
	textShader.Use();
	textShader.SetVector4f("_TextColor", component.color);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	component.DoRender();
}

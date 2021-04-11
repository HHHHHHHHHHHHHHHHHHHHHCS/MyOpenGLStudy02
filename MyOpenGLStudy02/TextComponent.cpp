#include "TextComponent.h"

#include <iostream>

#include "TextRenderer.h"

TextComponent::TextComponent()
{
}

TextComponent::TextComponent(TextRenderer* textRender)
{
	renderer = textRender;

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
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TextComponent::TextComponent(TextRenderer* textRender, std::string text, float x, float y, float scale, glm::vec4 color)
	: TextComponent(textRender)
{
	SetText(std::move(text), x, y, scale, color);
}

TextComponent::~TextComponent()
{
	renderer = nullptr;
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void TextComponent::SetText(std::string text, float _x, float _y, float _scale, glm::vec4 _color)
{
	startX = _x;
	startY = _y;
	scale = _scale;
	color = _color;
	SetText(std::move(text));
}

void TextComponent::SetText(std::string text)
{
	if (text == textContext)
	{
		return;
	}

	textContext = text;
	textLength = textContext.length();


	//我们已知的是有一些字形直接与顶部接触，如字符’H’、’T’或’X’。
	//那么我们通过接触顶部的字形的bearingY减去顶部不确定字形的bearingY来计算红色矢量的长度。
	//使用这种方法，我们依据字形顶部的点与顶部边差异的距离来向下推进字形。
	const float cHeight = static_cast<float>(this->renderer->characters['H'].bearing.y);
	const unsigned int textLength = text.length();
	GLfloat* vertices = new GLfloat[4 * 6 * textLength];
	unsigned int* indices = new unsigned int[6 * textLength];
	float x = startX, y = startY;

	unsigned int index = 0;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = this->renderer->characters[*c];

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

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6 * textLength, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * textLength, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] indices;
}

void TextComponent::Render()
{
	renderer->RenderTextComponent(*this);
}


void TextComponent::DoRender() const
{
	glBindVertexArray(this->VAO);

	glDrawElements(GL_TRIANGLES, 2 * 3 * textLength, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

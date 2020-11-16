#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "Texture2D.h"


class SpriteRenderer
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10)
	                , GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader shader;
	 GLboolean isInit;
	 GLuint quadVAO;

	 void InitRenderData();
};

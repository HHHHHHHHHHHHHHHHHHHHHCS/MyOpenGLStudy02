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

	glm::mat4 ModelMatrix(const glm::vec2& position, const glm::vec2& size, GLfloat rotate) const;

	void DrawSprite(Texture2D* texture, const glm::vec2& position, const glm::vec2& size = glm::vec2(10, 10)
	                , GLfloat rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));

private:
	Shader shader;
	GLboolean isInit;
	GLuint quadVAO;
	GLuint quadVBO;

	void InitRenderData();
};

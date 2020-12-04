#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader): isInit(false)
{
	this->shader = shader;
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &quadVAO);
}


void SpriteRenderer::InitRenderData()
{
	if (isInit)
	{
		return;
	}

	isInit = true;

	GLuint quadVBO;
	GLfloat vertices[] = {
		//位置	//纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,


		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::mat4 SpriteRenderer::ModelMatrix(const glm::vec2& position
                                      , const glm::vec2& size, GLfloat rotate) const
{
	glm::mat4 model{1};

	//position
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//rotation
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	//size
	model = glm::scale(model, glm::vec3(size, 1.0f));

	return model;
}

void SpriteRenderer::DrawSprite(const Texture2D& texture, const glm::vec2& position, const glm::vec2& size
                                , GLfloat rotate, const glm::vec3& color)
{
	this->shader.Use();

	const glm::mat4& model = ModelMatrix(position, size, rotate);

	this->shader.SetMatrix4x4("model", model);
	this->shader.SetVector3f("spriteColor", color);

	texture.Bind(0);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

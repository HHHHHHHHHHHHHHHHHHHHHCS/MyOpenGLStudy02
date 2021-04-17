#include "SpriteInstanceRenderer.h"

#include "DebugLog.h"

SpriteInstanceRenderer::SpriteInstanceRenderer(Shader& shader) : isInit(false)
{
	this->shader = shader;
	InitRenderData();
}

SpriteInstanceRenderer::~SpriteInstanceRenderer()
{
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &quadVAO);
}


void SpriteInstanceRenderer::InitRenderData()
{
	if (isInit)
	{
		return;
	}

	isInit = true;

	GLfloat vertices[] = {
		//位置	//纹理
		0.0f, 0.0f, 0.0f, 0.0f,
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(int)
	//                       , reinterpret_cast<void*>(0));
	// glVertexAttribDivisor(1, 1);
	//
	//
	// glEnableVertexAttribArray(2);
	// glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4)
	//                       , reinterpret_cast<void*>(0));
	//
	// glEnableVertexAttribArray(3);
	// glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4)
	//                       , reinterpret_cast<void*>(sizeof(glm::vec4)));
	//
	// glEnableVertexAttribArray(4);
	// glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
	//                       reinterpret_cast<void*>(2 * sizeof(glm::vec4)));
	//
	// glEnableVertexAttribArray(5);
	// glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
	//                       reinterpret_cast<void*>(3 * sizeof(glm::vec4)));
	//
	// glVertexAttribDivisor(2, 1);
	// glVertexAttribDivisor(3, 1);
	// glVertexAttribDivisor(4, 1);
	// glVertexAttribDivisor(5, 1);


	glBindVertexArray(0);
}

glm::mat4 SpriteInstanceRenderer::ModelMatrix(const glm::vec2& position
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

void SpriteInstanceRenderer::DrawSpriteInstance(Texture2D* texture, const glm::vec2& position, const glm::vec2& size
                                                , GLfloat rotate, const glm::vec3& color)
{
	this->shader.Use();

	const glm::mat4& model = ModelMatrix(position, size, rotate);

	this->shader.SetMatrix4x4("model", model);
	this->shader.SetVector3f("spriteColor", color);

	texture->Bind(0);

	glBindVertexArray(this->quadVAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float* imageType = new float[1];
	imageType[0] = position.x;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), &imageType[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float)
	                      , reinterpret_cast<void*>(0));
	glVertexAttribDivisor(1, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);


	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	delete[] imageType;
}

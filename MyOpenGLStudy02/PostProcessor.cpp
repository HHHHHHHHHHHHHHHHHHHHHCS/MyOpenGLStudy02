#include "PostProcessor.h"

#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
	: postProcessingShader(shader), texture(), width(width), height(height), confuse(GL_FALSE), chaos(GL_FALSE),
	  shake(GL_FALSE)
{
	//gen
	glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);

	//bind
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::PostProcessor: Failed to initialize MSFBO" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->texture.Generate(width, height, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::PostProcessor: Failed to initialize FBO" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//init
	this->InitRenderData();
	this->postProcessingShader.Use();
	this->postProcessingShader.SetInteger("scene", 0);

	/*
	const GLfloat offset = 1.0f / 300.0f;
	const GLfloat offsets[9][2] =
	{
		{-offset, offset}, // top-left
		{0.0f, offset}, // top-center
		{offset, offset}, // top-right
		{-offset, 0.0f}, // center-left
		{0.0f, 0.0f}, // center-center
		{offset, 0.0f}, // center - right
		{-offset, -offset}, // bottom-left
		{0.0f, -offset}, // bottom-center
		{offset, -offset} // bottom-right  
	};
	this->postProcessingShader.SetVector2fArray("offsets", sizeof(offsets) / sizeof(offsets[0]),
	                                            reinterpret_cast<const GLfloat*>(offsets));

	const GLint edge_kernel[9] =
	{
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	this->postProcessingShader.SetVector1iArray("edge_kernel", sizeof(edge_kernel),
	                                            reinterpret_cast<const GLint*>(edge_kernel));


	const GLfloat blur_kernel[9] =
	{
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	this->postProcessingShader.SetVector1iArray("blur_kernel", sizeof(blur_kernel),
	                                            reinterpret_cast<const GLint*>(blur_kernel));
	*/
}


void PostProcessor::InitRenderData()
{
	//用大三角形画
	GLuint VBO;
	const GLfloat vertices[] =
	{
		3.0f, -1.0f,
		-1.0f, 3.0f,
		-1.0f, -1.0f,
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void PostProcessor::BeginRender() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	// glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT); //| GL_DEPTH_BUFFER_BIT);
}


void PostProcessor::EndRender() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT,
	                  GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->postProcessingShader.Use();
	this->postProcessingShader.SetFloat("time", time);
	this->postProcessingShader.SetInteger("confuse", this->confuse);
	this->postProcessingShader.SetInteger("chaos", this->chaos);
	this->postProcessingShader.SetInteger("shake", this->shake);
	texture.Bind(0);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void PostProcessor::Update(GLfloat dt)
{
	if (shakeTimer > 0)
	{
		shakeTimer -= dt;
		if (shakeTimer <= 0)
		{
			this->shake = false;
		}
	}
}

void PostProcessor::DoShake(GLfloat time)
{
	shake = true;
	shakeTimer = time;
}

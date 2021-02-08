#include "ParticleGenerator.h"

#include <ctime>

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D* texture, GLuint amount)
	: shader(shader), texture(texture), amount(amount), lastUsedParticle(0)
{
	this->Init();
}


void ParticleGenerator::Init()
{
	GLfloat particleQuad[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
	//set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void*>(0));
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->amount; ++i)
	{
		this->particles.emplace_back(Particle{});
	}

	//生成随机种子 用于位置和color
	srand(time(nullptr));
}


void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset)
{
	//add new particles
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->FindUnusedParticle();
		this->RespawnParticle(this->particles[unusedParticle], object, offset);
	}

	//update
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle& p = this->particles[i];
		p.life -= dt;
		if (p.life > 0.0f)
		{
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	this->shader.Use();
	for (Particle &particle : particles)
	{
		if (particle.life > 0.0f)
		{
			texture->Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			//TODO:换成SpriteRender
		}
	}


	//don't forget reset
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


GLuint ParticleGenerator::FindUnusedParticle()
{
	//先根据顺序查找 后面的
	for (GLuint i = lastUsedParticle; i < this->amount; ++i)
	{
		if (this->particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	//在遍历之前的
	for (GLuint i = 0; i < lastUsedParticle; ++i)
	{
		if (this->particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	//全部都没有找到 直接覆盖使用0
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	//+ - 偏移   -5  +5
	const GLfloat random = ((rand() % 100) - 50) / 10.0f;
	//0.5 ~ 1.5
	const GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object.velocity * 0.1f;
}

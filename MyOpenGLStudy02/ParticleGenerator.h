#pragma once
#include <vector>
#include <glad/glad.h>

#include "Particle.h"
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D* texture, GLuint amount);

	Shader shader;
	
	//update
	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	//render
	void Draw();
private:
	std::vector<Particle> particles;
	Texture2D* texture;
	GLuint amount;
	GLuint VAO{};
	GLuint lastUsedParticle;
	void Init();
	GLuint FindUnusedParticle();
	void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

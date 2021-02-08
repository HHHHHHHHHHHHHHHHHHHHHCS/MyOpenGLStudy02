#pragma once
#include <vec2.hpp>
#include <vec4.hpp>
#include <glad/glad.h>

struct Particle
{
public:
	glm::vec2 position, velocity;
	glm::vec4 color;
	GLfloat life;

	Particle();
};

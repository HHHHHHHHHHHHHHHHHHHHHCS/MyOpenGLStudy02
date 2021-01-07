#pragma once

#include "GameObject.h"

class BallObject : public GameObject
{
public:
	GLfloat radius;
	GLboolean stuck;

	BallObject();
	BallObject(const GLfloat& width, const GLfloat& height, const glm::vec2& pos, const GLfloat& radius,
	           const glm::vec2& velocity, const Texture2D& sprite);

	glm::vec2 Move(const GLfloat& dt);
	void Reset(const glm::vec2& position, const glm::vec2& velocity);
};

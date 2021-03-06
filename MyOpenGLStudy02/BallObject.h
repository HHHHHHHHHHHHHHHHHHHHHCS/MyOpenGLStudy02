#pragma once

#include "GameObject.h"

class BallObject : public GameObject
{
public:
	static const glm::vec2 C_BallVelocity;
	static const GLfloat C_BallRadius;

	GLfloat radius;
	GLboolean stuck;
	GLboolean sticky;
	GLboolean passThrough;
	glm::vec2 mapSize;

	BallObject();
	BallObject(const glm::vec2& _mapSize, const glm::vec2& _pos, const GLfloat& _radius,
	           const glm::vec2& _velocity, Texture2D* _sprite);

	glm::vec2 Move(const GLfloat& dt);
	void Reset(const glm::vec2& _position, const glm::vec2& _velocity);
};

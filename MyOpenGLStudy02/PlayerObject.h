#pragma once
#include "GameObject.h"

class PlayerObject : public GameObject
{
public:
	static const glm::vec2 C_PlayerSize;
	static const GLfloat C_PlayerVelocity;

	glm::vec2 mapBorder;
	GLfloat velocity;

	PlayerObject(const glm::vec2& _mapSize, const Texture2D& playerTexture);

	void Move(const GLfloat& dir);
};

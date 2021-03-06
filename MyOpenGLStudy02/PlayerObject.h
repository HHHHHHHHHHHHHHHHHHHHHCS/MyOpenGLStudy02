#pragma once
#include "GameObject.h"

class Game;

class PlayerObject : public GameObject
{
public:
	static const glm::vec2 C_PlayerSize;
	static const GLfloat C_PlayerVelocity;

	GLfloat velocity;
	glm::vec2 mapSize;

	PlayerObject(const glm::vec2& _mapSize, Texture2D* playerTexture);

	void Reset();
	GLfloat Move(const GLfloat& dir);
};

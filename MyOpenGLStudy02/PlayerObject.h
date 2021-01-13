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

	PlayerObject(const glm::vec2& _mapSize, const Texture2D& playerTexture);
	~PlayerObject();
	
	GLfloat Move(const GLfloat& dir);
};

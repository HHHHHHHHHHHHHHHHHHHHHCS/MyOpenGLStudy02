#pragma once
#include "GameObject.h"

class PlayerObject : public GameObject
{
public:
	static const glm::vec2 C_PlayerSize;
	static const GLfloat C_PlayerVelocity;

	glm::vec2 mapBorder;
	GLfloat velocity;

	PlayerObject(const GLfloat& _width, const GLfloat& _height, const Texture2D& playerTexture);

	void Move(const GLfloat& dir);
};

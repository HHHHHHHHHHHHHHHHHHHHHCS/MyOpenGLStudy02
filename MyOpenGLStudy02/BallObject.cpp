#include "BallObject.h"

#include "Game.h"

BallObject::BallObject(const GLfloat& width, const GLfloat& height, const glm::vec2& pos, const GLfloat& radius,
	const glm::vec2& velocity, const Texture2D& sprite)
		:GameObject()
{
	
}

glm::vec2 BallObject::Move(const GLfloat& dt)
{
	return glm::vec2(0,0);
}


void BallObject::Reset(const glm::vec2& position, const glm::vec2& velocity)
{
	
}

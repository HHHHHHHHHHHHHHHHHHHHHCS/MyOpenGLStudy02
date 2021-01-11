#include "BallObject.h"

#include "Game.h"

const glm::vec2 BallObject::C_BallVelocity(100.0f, -350.0f);
const GLfloat BallObject::C_BallRadius(12.5f);


BallObject::BallObject(const glm::vec2& _mapSize, const glm::vec2& _pos, const GLfloat& _radius,
                       const glm::vec2& _velocity, const Texture2D& _sprite)
	: GameObject(_pos, glm::vec2(_radius * 2, _radius * 2), _sprite)
	  , radius(_radius), stuck(true), mapBorder(_mapSize)
{
}

glm::vec2 BallObject::Move(const GLfloat& dt)
{
	if (!stuck)
	{
		position = velocity * dt;

		if (position.x <= 0.0f)
		{
			velocity.x = -velocity.x;
			position.x = 0.0f;
		}
		else if (position.x + size.x >= mapBorder.x)
		{
			velocity.x = -velocity.x;
			position.x = mapBorder.x - size.x;
		}
		if (position.y + size.y >= mapBorder.y)
		{
			velocity.y = -velocity.y;
			position.y = mapBorder.y - size.y;
		}
		return position;
	}
}


void BallObject::Reset(const glm::vec2& _position, const glm::vec2& _velocity)
{
	this->position = _position;
	this->velocity = _velocity;
	this->stuck = true;
}

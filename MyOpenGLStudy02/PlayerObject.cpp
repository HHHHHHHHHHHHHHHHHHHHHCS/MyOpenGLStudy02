#include "PlayerObject.h"

const glm::vec2 PlayerObject::C_PlayerSize(100, 20);
const GLfloat PlayerObject::C_PlayerVelocity(500.0f);

PlayerObject::PlayerObject(const glm::vec2& _mapSize,  Texture2D* playerTexture)
	: GameObject(glm::vec2(_mapSize.x / 2 - C_PlayerSize.x / 2, C_PlayerSize.y)
	             , C_PlayerSize, playerTexture), velocity(C_PlayerVelocity), mapSize(_mapSize)
{
}

void PlayerObject::Reset()
{
	size = C_PlayerSize;
	position = glm::vec2(mapSize.x / 2 - C_PlayerSize.x / 2, C_PlayerSize.y);
	color = glm::vec3(1.0f);
}


GLfloat PlayerObject::Move(const GLfloat& dir)
{
	const GLfloat oldX = position.x;
	position.x = glm::clamp(position.x + velocity * dir, 0.0f, mapSize.x - size.x);
	return position.x - oldX;
}

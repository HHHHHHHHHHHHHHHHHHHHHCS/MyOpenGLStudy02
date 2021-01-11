#include "PlayerObject.h"

const glm::vec2 PlayerObject::C_PlayerSize(100, 20);
const GLfloat PlayerObject::C_PlayerVelocity(500.0f);

PlayerObject::PlayerObject(const glm::vec2& _mapSize, const Texture2D& playerTexture)
	: GameObject(glm::vec2(_mapSize.x / 2 - C_PlayerSize.x / 2, C_PlayerSize.y)
	             , C_PlayerSize, playerTexture), mapBorder(_mapSize), velocity(C_PlayerVelocity)
{
}


void PlayerObject::Move(const GLfloat& dir)
{
	position.x = glm::clamp(position.x + velocity* dir, 0.0f, mapBorder.x - size.x);
}

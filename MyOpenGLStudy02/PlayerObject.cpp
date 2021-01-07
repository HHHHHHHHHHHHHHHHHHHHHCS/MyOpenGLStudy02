#include "PlayerObject.h"

const glm::vec2 PlayerObject::C_PlayerSize(100, 20);
const GLfloat PlayerObject::C_PlayerVelocity(500.0f);

PlayerObject::PlayerObject(const GLfloat& _width,const GLfloat& _height, const Texture2D& playerTexture)
	: GameObject(glm::vec2(_width / 2 - C_PlayerSize.x / 2, C_PlayerSize.y)
	             , C_PlayerSize, playerTexture), mapBorder(0, _width), velocity(C_PlayerVelocity)
{
}


void PlayerObject::Move(const GLfloat& dir)
{
	position.x = glm::clamp(position.x + velocity* dir, 0.0f, mapBorder.y - size.x);
}

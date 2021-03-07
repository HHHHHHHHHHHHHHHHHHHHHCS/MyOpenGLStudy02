#include "PowerUp.h"

#include <iostream>

const glm::vec2 PowerUp::C_Size(60.0f, 20.0f);
const glm::vec2 PowerUp::C_Velocity(0.0f, 150.0f);
const GLfloat PowerUp::C_UpTime = 0.5f;


PowerUp::PowerUp(ItemType _itemType, glm::vec3 _color, GLfloat _duration, glm::vec2 _position, Texture2D* _texture):
	GameObject(_position, C_Size, _texture, _color, C_Velocity)
	, itemType(_itemType), duration(_duration), activated(false), upTimer(C_UpTime)
{
}

bool PowerUp::operator==(const PowerUp& other)
{
	return this == &other;
}


bool PowerUp::operator!=(const PowerUp& other)
{
	return this != &other;
}

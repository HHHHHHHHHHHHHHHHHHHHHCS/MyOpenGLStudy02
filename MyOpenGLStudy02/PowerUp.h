#pragma once
#include <vec2.hpp>

#include "GameObject.h"

class PowerUp : public GameObject
{
public:
	enum class ItemType
	{
		Speed,
		Sticky,
		Pass,
		Pad,
		Confuse,
		Chaos
	};

	const static glm::vec2 C_Size;
	const static glm::vec2 C_Velocity;
	const static GLfloat C_UpTime;

	ItemType itemType;
	GLfloat upTimer;
	GLfloat duration;
	GLboolean activated;

	PowerUp(ItemType _itemType, glm::vec3 _color, GLfloat _duration,
	        glm::vec2 _position, Texture2D* _texture);
};

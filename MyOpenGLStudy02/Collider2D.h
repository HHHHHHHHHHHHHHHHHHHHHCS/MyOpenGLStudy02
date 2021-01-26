#pragma once
#include <glad/glad.h>
#include "GameObject.h"
#include  "BallObject.h"
#include "Rigibody2D.h"

typedef std::tuple<GLboolean, Rigibody2D::Direction, glm::vec2> CollisionInfo;

class Collider2D
{
public:
	//检测碰撞
	static GLboolean CheckRectCollision(GameObject& one, GameObject& two);

	static CollisionInfo CheckBallCollision(BallObject& one, GameObject& two);
};

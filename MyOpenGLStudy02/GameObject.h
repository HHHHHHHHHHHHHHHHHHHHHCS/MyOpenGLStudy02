#pragma once
#include <glad/glad.h>
#include <vec2.hpp>
#include <vec3.hpp>

#include "SpriteRenderer.h"
#include "Texture2D.h"

class GameObject
{
public:
	//object state
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity; //速度
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid; //是不是坚硬的 不可摧毁的
	GLboolean destroyed = false;
	//render state
	Texture2D sprite;
	//constructor(s)
	GameObject();
	GameObject(const glm::vec2& _pos, const glm::vec2& _size, const Texture2D& _sprite,
	           const glm::vec3& _color = glm::vec3(1.0f)
	           , const glm::vec2& _velocity = glm::vec2(0.0f, 0.0f));

	//检测碰撞
	static GLboolean CheckCollision(GameObject& one, GameObject& two);
	
	//draw sprite
	virtual void Draw(SpriteRenderer& renderer);

};

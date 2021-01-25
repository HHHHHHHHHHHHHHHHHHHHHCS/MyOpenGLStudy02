#include "GameObject.h"

#include "DebugLog.h"

GameObject::GameObject()
	: position(0, 0), size(1, 1), velocity(0, 0), color(1, 1, 1)
	  , rotation(0), isSolid(false), sprite()
{
}

GameObject::GameObject(const glm::vec2& _pos, const glm::vec2& _size, const Texture2D& _sprite,
                       const glm::vec3& _color, const glm::vec2& _velocity)
	: position(_pos), size(_size), velocity(_velocity), color(_color)
	  , rotation(0), isSolid(false), destroyed(false), sprite(_sprite)
{
}

GLboolean GameObject::CheckCollision(GameObject& one, GameObject& two)
{
	//x轴方向碰撞
	const GLboolean collisionX = one.position.x + one.size.x >= two.position.x
		&& two.position.x + two.size.x >= one.position.x;

	const GLboolean collisionY = one.position.y + one.size.y >= two.position.y
		&& two.position.y + two.size.y >= one.position.y;
	return collisionX && collisionY;
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}


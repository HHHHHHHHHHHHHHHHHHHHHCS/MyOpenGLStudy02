#include "GameObject.h"

GameObject::GameObject()
	: position(0, 0), size(1, 1), velocity(0, 0), rotation(0)
	  , isSolid(false), destroyed(false), sprite()
{
}

GameObject::GameObject(const glm::vec2& _pos, const glm::vec2& _size, const Texture2D& _sprite,
                       const glm::vec3& _color, const glm::vec2& _velocity)
	: position(_pos), size(_size), velocity(_velocity), color(_color)
	  , rotation(0), isSolid(false), destroyed(false), sprite(_sprite)
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}

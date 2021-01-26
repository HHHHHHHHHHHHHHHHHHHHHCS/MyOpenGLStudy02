#include "Collider2D.h"

#include <tuple>


GLboolean Collider2D::CheckRectCollision(GameObject& one, GameObject& two)
{
	//x轴方向碰撞
	const GLboolean collisionX = one.position.x + one.size.x >= two.position.x
		&& two.position.x + two.size.x >= one.position.x;

	const GLboolean collisionY = one.position.y + one.size.y >= two.position.y
		&& two.position.y + two.size.y >= one.position.y;
	return collisionX && collisionY;
}

CollisionInfo Collider2D::CheckBallCollision(BallObject& one, GameObject& two)
{
	//获取圆的中心
	glm::vec2 center(one.position + one.radius);
	//计算AABB的信息(中心,半边长)
	const glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	const glm::vec2 aabb_center(
		two.position.x + aabb_half_extents.x,
		two.position.y + aabb_half_extents.y
	);
	//获取两个中心的差矢量
	glm::vec2 difference = center - aabb_center;
	const glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	//AABB_center加上clamped 这样就得到了矩形离圆最近点
	const glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	if (glm::dot(difference, difference) <= one.radius * one.radius)
	{
		return std::make_tuple(GL_TRUE, Rigibody2D::VectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(false, Rigibody2D::Direction::UP, glm::vec2(0, 0));
	}
}

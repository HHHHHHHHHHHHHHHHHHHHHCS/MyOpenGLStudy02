#pragma once
#include <vec2.hpp>

class Rigibody2D
{
public:
	enum class Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
	};
	
	static Direction VectorDirection(glm::vec2 target);
};

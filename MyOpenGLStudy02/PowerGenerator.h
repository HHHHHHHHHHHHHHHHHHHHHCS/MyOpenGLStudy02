#pragma once
#include <vector>

#include "PowerUp.h"
#include "ResourceManager.h"


class PowerGenerator
{
public:
	std::vector<PowerUp> powerUps;

	GLboolean ShouldSpawn(GLuint chance);
	void SpawnPowerUps(ResourceManager* resourceManager,GameObject& block );
	void UpdatePowerUps(GLfloat dt);
};

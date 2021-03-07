#pragma once
#include <vector>

#include "PowerUp.h"
#include "ResourceManager.h"

class Game;

class PowerUpGenerator
{
public:
	std::vector<PowerUp> powerUps;

	void SpawnPowerUps(ResourceManager* resourceManager, GameObject& block);
	void UpdatePowerUps(GLfloat dt, Game* game);
	void ActivePowerUp(PowerUp& powerUp, Game* game);
	void Draw(SpriteRenderer& spriteRenderer);
	void Reset();

private:
	GLboolean ShouldSpawn(GLuint chance);
};

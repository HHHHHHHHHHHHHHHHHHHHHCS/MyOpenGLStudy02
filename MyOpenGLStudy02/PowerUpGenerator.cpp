#include "PowerUpGenerator.h"

#include "Collider2D.h"
#include "DebugLog.h"
#include "Game.h"

GLboolean PowerUpGenerator::ShouldSpawn(GLuint chance)
{
	const GLuint random = rand() % chance;
	return random == 0;
}

void PowerUpGenerator::SpawnPowerUps(ResourceManager* resourceManager, GameObject& block)
{
	this->powerUps.emplace_back(
		PowerUp(PowerUp::ItemType::Chaos, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position,
		        resourceManager->GetTexture(ConstConfigure::Image_ChaosKey)));
	return;
	if (ShouldSpawn(75)) // 1/75的几率
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Speed, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_SpeedKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Sticky, glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_StickyKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Pass, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_PassKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Pad, glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_PadKey)));
	else if (ShouldSpawn(15)) // 负面道具减少生成概率
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Confuse, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ConfuseKey)));
	else if (ShouldSpawn(15))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Chaos, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ChaosKey)));
}

void PowerUpGenerator::UpdatePowerUps(GLfloat dt, Game* game)
{
	PlayerObject* player = game->player;

	const int count = static_cast<int>(powerUps.size());
	for (int i = count - 1; i >= 0; i--)
	{
		auto&& powerUp = powerUps[i];
		if (!powerUp.destroyed)
		{
			if (powerUp.upTimer > 0)
			{
				powerUp.position += powerUp.upTimer * powerUp.velocity * dt;
				powerUp.upTimer -= dt;
			}
			else
			{
				powerUp.position -= powerUp.velocity * dt;
			}


			if (powerUp.position.y < 0)
			{
				powerUp.destroyed = true;
			}
			else if (Collider2D::CheckRectCollision(*player, powerUp))
			{
				ActivePowerUp(powerUp, game);
			}
		}


		// if (powerUp.destroyed)
		// {
		// 	powerUps.erase(powerUps.begin() + i);
		// }

		//TODO:结束 删除
	}
}

void PowerUpGenerator::ActivePowerUp(PowerUp& powerUp, Game* game)
{
	game->ActivatePowerUp(powerUp);
	powerUp.destroyed = true;
	powerUp.activated = true;

	const auto itemType = powerUp.itemType;

	if (itemType == PowerUp::ItemType::Sticky || itemType == PowerUp::ItemType::Pass
		|| itemType == PowerUp::ItemType::Confuse || itemType == PowerUp::ItemType::Chaos)
	{
		//再之后统一删除
		for (auto&& item : activePowerUps)
		{
			if (item.itemType == itemType)
			{
				item.activated = false;
				item.duration = 0;
			}
		}
	}

	activePowerUps.emplace_back(powerUp);
}

void PowerUpGenerator::Draw(SpriteRenderer& spriteRenderer)
{
	for (auto&& powerUp : powerUps)
	{
		if (!powerUp.destroyed)
		{
			powerUp.Draw(spriteRenderer);
		}
	}
}

void PowerUpGenerator::Reset()
{
	powerUps.clear();
	activePowerUps.clear();
}

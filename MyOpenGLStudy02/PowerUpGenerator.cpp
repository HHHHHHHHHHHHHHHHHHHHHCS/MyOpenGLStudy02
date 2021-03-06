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
		PowerUp(PowerUp::ItemType::Speed, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position,
			resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	return;
	//TODO:刚生产就被吃掉了
	if (ShouldSpawn(75)) // 1/75的几率
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Speed, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Sticky, glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Pass, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	else if (ShouldSpawn(75))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Pad, glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	else if (ShouldSpawn(15)) // 负面道具被更频繁地生成
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Confuse, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
	else if (ShouldSpawn(15))
		this->powerUps.emplace_back(
			PowerUp(PowerUp::ItemType::Chaos, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position,
			        resourceManager->GetTexture(ConstConfigure::Image_ParticleKey)));
}

void PowerUpGenerator::UpdatePowerUps(GLfloat dt, Game* game)
{
	BallObject* ball = game->ball;

	const int count = static_cast<int>(powerUps.size());
	for (int i = count - 1; i >= 0; i--)
	{
		auto&& powerUp = powerUps[i];
		if (!powerUp.destroyed)
		{
			powerUp.position += powerUp.velocity * dt;


			if (powerUp.position.y < 0)
			{
				powerUp.destroyed = true;
			}
			else
			{
				auto collisionInfo = Collider2D::CheckBallCollision(*ball, powerUp);
				if (std::get<0>(collisionInfo))
				{
					DebugLog::Print("a");
					game->ActivatePowerUp(powerUp);
					powerUp.destroyed = true;
					powerUp.activated = true;
				}
			}
		}


		// if (powerUp.destroyed)
		// {
		// 	powerUps.erase(powerUps.begin() + i);
		// }
	}
}

void PowerUpGenerator::Draw( SpriteRenderer& spriteRenderer)
{
	for (auto && powerUp : powerUps)
	{
		if(!powerUp.destroyed)
		{
			powerUp.Draw(spriteRenderer);
		}
	}
}

void PowerUpGenerator::Reset()
{
	powerUps.clear();
}

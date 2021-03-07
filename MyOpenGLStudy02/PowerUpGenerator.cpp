#include "PowerUpGenerator.h"

#include <iostream>


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
		else if (powerUp.activated)
		{
			powerUp.duration -= dt;
			if (powerUp.duration <= 0)
			{
				powerUp.destroyed = true;
			}
		}

		if (powerUp.destroyed)
		{
			if (powerUp.activated)
			{
				if (powerUp.duration <= 0)
				{
					game->StopPowerUp(powerUp);
					powerUps.erase(powerUps.begin() + i);
				}
			}
			else
			{
				powerUps.erase(powerUps.begin() + i);
			}
		}

		// remove_if的参数是迭代器，前两个参数表示迭代的起始位置和这个起始位置所对应的停止位置。
		// 最后一个参数：传入一个回调函数，如果回调函数返回为真，则将当前所指向的参数移到尾部。
		// 返回值是被移动区域的首个元素。
		// 
		// this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		// 	[](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
		// ), this->PowerUps.end());
	}
}

void PowerUpGenerator::ActivePowerUp(PowerUp& powerUp, Game* game)
{
	game->ActivatePowerUp(powerUp);
	powerUp.destroyed = true;
	powerUp.activated = true;

	const auto itemType = powerUp.itemType;
	//如果有已经激活的了  把老的废弃了
	if (itemType == PowerUp::ItemType::Sticky || itemType == PowerUp::ItemType::Pass
		|| itemType == PowerUp::ItemType::Confuse || itemType == PowerUp::ItemType::Chaos)
	{
		//再之后统一删除
		for (auto&& item : powerUps)
		{
			if (item.activated && item.itemType == itemType && item != powerUp)
			{
				item.activated = false;
				item.destroyed = true;
				item.duration = 0;
			}
		}
	}

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
}

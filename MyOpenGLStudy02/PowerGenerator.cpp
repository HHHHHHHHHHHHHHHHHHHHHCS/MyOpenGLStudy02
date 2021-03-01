#include "PowerGenerator.h"

#include "Program.h"

GLboolean PowerGenerator::ShouldSpawn(GLuint chance)
{
	const GLuint random = rand() % chance;
	return random == 0;
}

void PowerGenerator::SpawnPowerUps(ResourceManager* resourceManager, GameObject& block)
{
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

	resourceManager = nullptr;
}

void PowerGenerator::UpdatePowerUps(GLfloat dt)
{
}

#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "GameLevel.h"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "PowerUpGenerator.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "PostProcessor.h"
#include "TextRenderer.h"
#include "irrKlang.h"


#include "PlayerObject.h"
#include "BallObject.h"
#include "KeyInput.h"
#include "PowerUp.h"


//游戏的状态
enum class GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	static const unsigned int C_PlayerLife;


	Camera camera{};
	KeyInput keyInput{};
	ResourceManager resourceManager{};
	SpriteRenderer* spriteRenderer = nullptr;
	ParticleGenerator* particleGenerator = nullptr;
	PowerUpGenerator* powerUpGenerator = nullptr;
	PostProcessor* postProcessor = nullptr;
	TextRenderer* textRenderer = nullptr;
	irrklang::ISoundEngine* soundEngine = nullptr;


	PlayerObject* player = nullptr;
	BallObject* ball = nullptr;

	//关卡相关
	std::vector<GameLevel> levels{};
	GLuint level;
	GLuint life;


	// 游戏状态
	GameState state ;

	GLuint width, height;
	glm::vec2 mapSize;

	Game(GLuint _width, GLuint _height);
	~Game();
	//初始化游戏状态（加载所有的着色器/纹理/关卡）
	void Init();
	//游戏循环
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void StopPowerUp(PowerUp& powerUp) const;


	void CheckCollisions();
	void ActivatePowerUp(PowerUp& power) const;
	void CheckFail();
	void CheckWin();

	void UpdateParticles();

private:

	void InitRes();
	void ReLoadLevel(int _level);
	void ResetPlayer();
};

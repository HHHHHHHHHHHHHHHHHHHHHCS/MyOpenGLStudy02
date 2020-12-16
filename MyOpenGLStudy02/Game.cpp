#include "Game.h"

#include <iostream>


Game::Game(GLuint _width, GLuint _height)
{
	memset(keys, 0, sizeof(keys));
	width = _width;
	height = _height;
	GLuint halfWidth = _width / 2;
	GLuint halfHeight = _height / 2;

	camera.Init(glm::vec3(halfWidth, halfHeight, 0), glm::vec3(0, 0, 0), glm::vec3(halfWidth, halfHeight, 1));
}

Game::~Game()
{
}

void Game::Init()
{
	InitRes();
	state = GameState::GAME_ACTIVE;
	//序号正常要-1
	this->level = 2;
	Shader spriteShader = resourceManager.GetShader(ConstConfigure::Shader_SpriteKey);
	spriteShader.Use().SetInteger("image", 0);
	spriteShader.SetMatrix4x4("viewProjection", camera.GetViewProjection());
	spriteRenderer = new SpriteRenderer(spriteShader);
}

void Game::InitRes()
{
	resourceManager.LoadShader(ConstConfigure::Shader_SpriteKey, ConstConfigure::Shader_SpritePath);

	resourceManager.LoadTexture(ConstConfigure::Image_BackgroundKey, ConstConfigure::Image_BackgroundPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockKey, ConstConfigure::Image_BlockPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockSolidKey, ConstConfigure::Image_BlockSolidPath);

	GameLevel one;
	one.Load(ConstConfigure::Level_1Path, this->width, static_cast<GLuint>(this->height * 0.5));
	this->levels.emplace_back(one);
	GameLevel two;
	two.Load(ConstConfigure::Level_2Path, this->width, static_cast<GLuint>(this->height * 0.5));
	this->levels.emplace_back(two);
	GameLevel three;
	three.Load(ConstConfigure::Level_3Path, this->width, static_cast<GLuint>(this->height * 0.5));
	this->levels.emplace_back(three);
	GameLevel four;
	four.Load(ConstConfigure::Level_4Path, this->width, static_cast<GLuint>(this->height * 0.5));
	this->levels.emplace_back(four);
}


void Game::ProcessInput(GLdouble dt)
{
}

void Game::Update(GLdouble dt)
{
}

void Game::Render()
{
	if (this->state == GameState::GAME_ACTIVE)
	{
		spriteRenderer->DrawSprite(resourceManager.GetTexture(ConstConfigure::Image_BackgroundKey)
		                           , glm::vec2(0, 0), glm::vec2(this->width, this->height), 0);
		//序号正常要-1
		this->levels[this->level - 1].Draw(*spriteRenderer);
	}
}

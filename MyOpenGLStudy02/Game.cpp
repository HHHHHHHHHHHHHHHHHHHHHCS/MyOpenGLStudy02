#include "Game.h"

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
	state = GameState::GAME_ACTIVE;
	Shader spriteShader = resourceManager.LoadShader("sprite", "sprite");
	spriteShader.Use().SetInteger("image", 0);
	spriteShader.SetMatrix4x4("viewProjection", camera.GetViewProjection());
	resourceManager.LoadTexture("face", "awesomeface.png");
	spriteRenderer = new SpriteRenderer(spriteShader);
}

void Game::ProcessInput(GLdouble dt)
{
}

void Game::Update(GLdouble dt)
{
}

void Game::Render()
{
	spriteRenderer->DrawSprite(resourceManager.GetTexture("face"), glm::vec2(200, 200)
	                           , glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

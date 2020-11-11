#include "Game.h"

#include <iostream>

Game::Game(GLuint width, GLuint height)
{
	GLuint halfWidth = width / 2;
	GLuint halfHeight = height / 2;

	camera.Init(glm::vec3(halfWidth, halfHeight, 0), glm::vec3(0, 0, 0), glm::vec3(halfWidth, halfHeight, 1));

	glm::vec4 pos(0, 0, 0.5f, 1.0);
	pos = camera.GetViewProjection() * pos;
	std::cout << pos.x << "|" << pos.y << "|" << pos.z << "|" << pos.w << std::endl;

	pos = glm::vec4(400, 300, 0.0f, 1.0);
	pos = camera.GetViewProjection() * pos;
	std::cout << pos.x << "|" << pos.y << "|" << pos.z << "|" << pos.w << std::endl;
	
	pos = glm::vec4(800, 600, -1.0f, 1.0);
	pos = camera.GetViewProjection() * pos;
	std::cout << pos.x << "|" << pos.y << "|" << pos.z << "|" << pos.w << std::endl;
}

Game::~Game()
{
}

void Game::Init()
{
}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
}

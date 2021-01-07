﻿#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "GameLevel.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

class PlayerObject;

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
	Camera camera;
	ResourceManager resourceManager;
	SpriteRenderer* spriteRenderer;

	PlayerObject* player;

	//关卡相关
	std::vector<GameLevel> levels;
	GLuint level;

	// 游戏状态
	GameState state;
	GLboolean keys[GLFW_KEY_LAST + 1];
	GLuint width, height;


	Game(GLuint _width, GLuint _height);
	~Game();
	//初始化游戏状态（加载所有的着色器/纹理/关卡）
	void Init();
	//游戏循环
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

private:

	void InitRes();
};

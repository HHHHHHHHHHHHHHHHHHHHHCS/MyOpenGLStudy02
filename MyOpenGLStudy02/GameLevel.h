#pragma once

#include <vector>

#include "GameObject.h"

class GameLevel
{
public:
	std::vector<GameObject> bricks;

	GameLevel()
	{
	}

	//从文件中加载关卡
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	//渲染关卡
	void Draw(const SpriteRenderer& renderer);
	//检查一个关卡是否完成(所有非坚硬的块都被摧毁)
	GLboolean IsCompleted();
private:
	void Init(const std::vector<std::vector<GLuint>>& tileData, GLuint levelWidth, GLuint levelHeight);
};

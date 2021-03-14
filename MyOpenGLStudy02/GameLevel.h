#pragma once

#include <vector>

#include "ConstConfigure.h"
#include "GameObject.h"

class GameLevel
{
public:
	std::vector<GameObject> bricks;

	unsigned int breakCount = 0;
	
	GameLevel();
	~GameLevel();


	//从文件中加载关卡
	void Load(const std::string& file, GLuint levelWidth, GLuint levelHeight, const std::string& directory = ConstConfigure::Level_Directory);
	//渲染关卡
	void Draw(SpriteRenderer& renderer);
	//检查一个关卡是否完成(所有非坚硬的块都被摧毁)
	GLboolean IsCompleted();
private:
	void Init(const std::vector<std::vector<GLuint>>& tileData, GLuint levelWidth, GLuint levelHeight);
};

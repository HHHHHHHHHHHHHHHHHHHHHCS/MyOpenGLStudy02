#include "GameLevel.h"

#include <fstream>
#include <sstream>


#include "Program.h"
#include "ResourceManager.h"

//数字0：无砖块，表示关卡中空的区域
//数字1：一个坚硬的砖块，不可被摧毁
//大于1的数字：一个可被摧毁的砖块，不同的数字区分砖块的颜色

//从文件中加载关卡
void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
	//清空过期的数据
	this->bricks.clear();
	//从文本中加载
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		//读取关卡文件的每一行
		while (std::getline(fstream, line))
		{
			//读取被空格分开的每一个数字
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (!tileData.empty())
		{
			this->Init(tileData, levelWidth, levelHeight);
		}
	}
}

//渲染关卡
void GameLevel::Draw(const SpriteRenderer& renderer)
{
}

//检查一个关卡是否完成(所有非坚硬的块都被摧毁)
GLboolean GameLevel::IsCompleted()
{
}

//初始化数据
void GameLevel::Init(const std::vector<std::vector<GLuint>>& tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	//步长
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / static_cast<GLfloat>(height);

	glm::vec2 size(unit_width, unit_height);

	
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec3 color = glm::vec3(1.0f);//默认为白色
			std::string texturePath;
			GLboolean isSolid = GL_FALSE;
			
			if (tileData[y][x] == 1)
			{
				isSolid = GL_TRUE;
				color = glm::vec3(0.8f, 0.8f, 0.7f);
				texturePath = "block_solid";
			}
			else if(tileData[y][x] > 1)
			{
				if (tileData[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				texturePath = "block";
			}

			GameObject obj = GameObject(pos, size
				, Program::instance->game.resourceManager.GetTexture(texturePath)
				, color);
			this->bricks.emplace_back(obj);
		}
	}
}

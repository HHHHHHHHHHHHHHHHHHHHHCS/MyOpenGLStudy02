#include "GameLevel.h"

#include <fstream>
#include <sstream>


#include "Program.h"
#include "ResourceManager.h"

//数字0：无砖块，表示关卡中空的区域
//数字1：一个坚硬的砖块，不可被摧毁
//大于1的数字：一个可被摧毁的砖块，不同的数字区分砖块的颜色

GameLevel::GameLevel() = default;

GameLevel::~GameLevel() = default;

//从文件中加载关卡
void GameLevel::Load(const std::string& file, GLuint levelWidth, GLuint levelHeight,
                     const std::string& directory)
{
	//清空过期的数据
	this->bricks.clear();
	//从文本中加载
	GLuint tileCode;
	std::string line;
	std::ifstream fstream(directory + file);
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
void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->bricks)
	{
		if (!tile.destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

void GameLevel::DrawInstance(SpriteInstanceRenderer& instanceRenderer)
{
	for (GameObject& tile : this->bricks)
	{
		if (!tile.destroyed)
		{
			tile.DrawInstance(instanceRenderer);
		}
	}
}

//检查一个关卡是否完成(所有非坚硬的块都被摧毁)
GLboolean GameLevel::IsCompleted()
{
	return true;
}

//初始化数据
void GameLevel::Init(const std::vector<std::vector<GLuint>>& tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = static_cast<GLuint>(tileData.size());
	GLuint width = static_cast<GLuint>(tileData[0].size());
	//步长
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / static_cast<GLfloat>(height);

	glm::vec2 size(unit_width, unit_height);
	breakCount = 0;

	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			int val = tileData[y][x];

			if (val == 0)
			{
				continue;
			}

			glm::vec2 pos(x * unit_width,
			              2 * levelHeight - (unit_height * (y + 1)));
			glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); //默认为白色
			std::string texturePath;
			GLboolean isSolid = GL_FALSE;

			if (val == 1)
			{
				isSolid = GL_TRUE;
				color = glm::vec3(0.8f, 0.8f, 0.7f);
				texturePath = ConstConfigure::Image_BlockSolidKey;
			}
			else if (val > 1)
			{
				breakCount++;
				if (val == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (val == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (val == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (val == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				texturePath = ConstConfigure::Image_BlockKey;
			}

			GameObject obj(pos, size
			               , Program::instance->game.resourceManager.GetTexture(texturePath)
			               , color);
			obj.isSolid = isSolid;
			this->bricks.emplace_back(obj);
		}
	}
}

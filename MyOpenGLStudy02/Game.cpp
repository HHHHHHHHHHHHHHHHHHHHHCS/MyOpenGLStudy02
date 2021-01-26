#include "Game.h"


#include "BallObject.h"
#include "Collider2D.h"
#include "DebugLog.h"
#include "PlayerObject.h"


Game::Game(GLuint _width, GLuint _height)
{
	memset(keys, 0, sizeof(keys));
	width = _width;
	height = _height;
	mapSize = glm::vec2(_width, _height);
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
	//数组从0开始    关卡的名字从1开始
	this->level = 0;
	Shader spriteShader = resourceManager.GetShader(ConstConfigure::Shader_SpriteKey);
	spriteShader.Use().SetInteger("image", 0);
	spriteShader.SetMatrix4x4("viewProjection", camera.GetViewProjection());
	spriteRenderer = new SpriteRenderer(spriteShader);
	player = new PlayerObject(mapSize, resourceManager.GetTexture(ConstConfigure::Image_PaddleKey));
	const glm::vec2 ballPos = player->position + glm::vec2(player->size.x / 2 - BallObject::C_BallRadius,
	                                                       2 * BallObject::C_BallRadius);
	ball = new BallObject(mapSize, ballPos, BallObject::C_BallRadius, BallObject::C_BallVelocity,
	                      resourceManager.GetTexture(ConstConfigure::Image_BallKey));
}

void Game::InitRes()
{
	resourceManager.LoadShader(ConstConfigure::Shader_SpriteKey, ConstConfigure::Shader_SpritePath);

	resourceManager.LoadTexture(ConstConfigure::Image_BackgroundKey, ConstConfigure::Image_BackgroundPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockKey, ConstConfigure::Image_BlockPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockSolidKey, ConstConfigure::Image_BlockSolidPath);
	resourceManager.LoadTexture(ConstConfigure::Image_PaddleKey, ConstConfigure::Image_PaddlePath);
	resourceManager.LoadTexture(ConstConfigure::Image_BallKey, ConstConfigure::Image_BallPath);

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


void Game::ProcessInput(GLfloat dt)
{
	if (this->state == GameState::GAME_ACTIVE)
	{
		GLfloat moveX = 0;
		GLfloat velocity = 0;
		if (this->keys[GLFW_KEY_A])
		{
			velocity -= dt;
		}
		if (this->keys[GLFW_KEY_D])
		{
			velocity += dt;
		}
		if (velocity != 0)
		{
			moveX = player->Move(velocity);
		}
		if (ball->stuck)
		{
			ball->position.x += moveX;
		}
		if (this->keys[GLFW_KEY_SPACE])
		{
			ball->stuck = false;
		}
	}
}

void Game::Update(GLfloat dt)
{
	ball->Move(dt);
	this->DoCollisions();
}

void Game::Render()
{
	if (this->state == GameState::GAME_ACTIVE)
	{
		spriteRenderer->DrawSprite(resourceManager.GetTexture(ConstConfigure::Image_BackgroundKey)
		                           , glm::vec2(0, 0), glm::vec2(this->width, this->height), 0);
		this->levels[this->level].Draw(*spriteRenderer);

		player->Draw(*spriteRenderer);
		ball->Draw(*spriteRenderer);
	}
}

void Game::DoCollisions()
{
	for (GameObject& tile : this->levels[this->level].bricks)
	{
		if (!tile.destroyed)
		{
			CollisionInfo collisionInfo = Collider2D::CheckBallCollision(*ball, tile);
			if (std::get<0>(collisionInfo))
			{
				if (!tile.isSolid)
				{
					tile.destroyed = GL_TRUE;
				}
				//碰撞处理
				Rigibody2D::Direction dir = std::get<1>(collisionInfo);
				glm::vec2 diff_vector = std::get<2>(collisionInfo);
				if (dir == Rigibody2D::Direction::LEFT || dir == Rigibody2D::Direction::RIGHT)
				{
					ball->velocity.x = -ball->velocity.x; //反转水平速度
					//重新定位  先找出内嵌多少距离
					GLfloat penetration = ball->radius - std::abs(diff_vector.x);
					if (dir == Rigibody2D::Direction::LEFT)
					{
						ball->position.x += penetration;
					}
					else
					{
						ball->position.x -= penetration;
					}
				}
				else //垂直方向
				{
					ball->velocity.y = -ball->velocity.y; //反转垂直速度
					//重新定位  先找出内嵌多少距离
					GLfloat penetration = ball->radius - std::abs(diff_vector.y);
					if (dir == Rigibody2D::Direction::UP)
					{
						ball->position.y += penetration;
					}
					else
					{
						ball->position.y -= penetration;
					}
				}
			}
		}
	}
	//TODO:
	//CollisionInfo result = Collider2D::CheckBallCollision(*ball, *player);
}

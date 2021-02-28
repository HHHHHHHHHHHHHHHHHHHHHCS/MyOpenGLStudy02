#include "Game.h"

#include <iostream>


#include "BallObject.h"
#include "Collider2D.h"
#include "DebugLog.h"
#include "PlayerObject.h"
#include "Program.h"


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
	delete postProcessor;
	delete spriteRenderer;
	delete player;
	delete ball;
	delete particleGenerator;
}

void Game::Init()
{
	InitRes();
	state = GameState::GAME_ACTIVE;
	//数组从0开始    关卡的名字从1开始
	this->level = 0;
	postProcessor = new PostProcessor{resourceManager.GetShader(ConstConfigure::Shader_PostProcessKey), width, height};
	Shader spriteShader = resourceManager.GetShader(ConstConfigure::Shader_SpriteKey);
	spriteShader.Use().SetInteger("image", 0);
	spriteShader.SetMatrix4x4("viewProjection", camera.GetViewProjection());
	spriteRenderer = new SpriteRenderer(spriteShader);
	player = new PlayerObject(mapSize, resourceManager.GetTexture(ConstConfigure::Image_PaddleKey));
	const glm::vec2 ballPos = player->position + glm::vec2(player->size.x / 2 - BallObject::C_BallRadius,
	                                                       PlayerObject::C_PlayerSize.y);
	ball = new BallObject(mapSize, ballPos, BallObject::C_BallRadius, BallObject::C_BallVelocity,
	                      resourceManager.GetTexture(ConstConfigure::Image_BallKey));

	particleGenerator = new ParticleGenerator(
		resourceManager.GetShader(ConstConfigure::Shader_ParticleKey)
		, resourceManager.GetTexture(ConstConfigure::Image_ParticleKey), 500
	);
	particleGenerator->shader.Use();
	particleGenerator->shader.SetMatrix4x4("viewProjection", camera.GetViewProjection());
}

void Game::InitRes()
{
	//load 这一块其实可以放到json/xml 里面做
	resourceManager.LoadShader(ConstConfigure::Shader_SpriteKey, ConstConfigure::Shader_SpritePath);
	resourceManager.LoadShader(ConstConfigure::Shader_ParticleKey, ConstConfigure::Shader_ParticlePath);
	resourceManager.LoadShader(ConstConfigure::Shader_PostProcessKey, ConstConfigure::Shader_PostProcessPath);

	resourceManager.LoadTexture(ConstConfigure::Image_BackgroundKey, ConstConfigure::Image_BackgroundPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockKey, ConstConfigure::Image_BlockPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockSolidKey, ConstConfigure::Image_BlockSolidPath);
	resourceManager.LoadTexture(ConstConfigure::Image_PaddleKey, ConstConfigure::Image_PaddlePath);
	resourceManager.LoadTexture(ConstConfigure::Image_BallKey, ConstConfigure::Image_BallPath);
	resourceManager.LoadTexture(ConstConfigure::Image_ParticleKey, ConstConfigure::Image_ParticlePath);

	GameLevel one;
	this->levels.emplace_back(one);
	ReLoadLevel(0);
	GameLevel two;
	this->levels.emplace_back(two);
	ReLoadLevel(1);
	GameLevel three;
	this->levels.emplace_back(three);
	ReLoadLevel(2);
	GameLevel four;
	this->levels.emplace_back(four);
	ReLoadLevel(3);
}

void Game::ReLoadLevel(int _level)
{
	std::string path;
	switch (_level)
	{
	case 0:
		path = ConstConfigure::Level_1Path;
		break;
	case 1:
		path = ConstConfigure::Level_2Path;
		break;
	case 2:
		path = ConstConfigure::Level_3Path;
		break;
	case 3:
		path = ConstConfigure::Level_4Path;
		break;
	default:
		DebugLog::Print(_level + "is not find!");
		return;
	}

	levels[_level].Load(path, this->width, static_cast<GLuint>(this->height * 0.5));
}

void Game::ResetPlayer() const
{
	player->ResetPos();
	const glm::vec2 ballPos = player->position + glm::vec2(player->size.x / 2 - BallObject::C_BallRadius,
	                                                       PlayerObject::C_PlayerSize.y);
	ball->Reset(ballPos, BallObject::C_BallVelocity);
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
	particleGenerator->Update(dt, *ball, 2, glm::vec2(ball->radius / 2));
	this->CheckCollisions();
	CheckFail();
	postProcessor->Update(dt);
}

void Game::Render()
{
	if (this->state != GameState::GAME_ACTIVE)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		// glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT); //| GL_DEPTH_BUFFER_BIT);
	}
	else if (this->state == GameState::GAME_ACTIVE)
	{
		postProcessor->BeginRender();

		spriteRenderer->DrawSprite(resourceManager.GetTexture(ConstConfigure::Image_BackgroundKey)
		                           , glm::vec2(0, 0), glm::vec2(this->width, this->height), 0);
		this->levels[this->level].Draw(*spriteRenderer);

		player->Draw(*spriteRenderer);
		if (!ball->stuck)
		{
			particleGenerator->Draw();
		}
		ball->Draw(*spriteRenderer);

		postProcessor->EndRender();

		postProcessor->Render(glfwGetTime());
	}
}

void Game::CheckCollisions()
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
				else
				{
					postProcessor->DoShake(0.05f);
				}
				//碰撞处理
				Rigibody2D::Direction dir = std::get<1>(collisionInfo);
				const glm::vec2 diff_vector = std::get<2>(collisionInfo);
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
					const GLfloat penetration = ball->radius - std::abs(diff_vector.y);
					if (dir == Rigibody2D::Direction::UP)
					{
						ball->position.y -= penetration;
					}
					else
					{
						ball->position.y += penetration;
					}
				}
			}
		}
	}
	CollisionInfo result = Collider2D::CheckBallCollision(*ball, *player);
	if (!ball->stuck && std::get<0>(result))
	{
		//检测碰到了挡板的哪个位置,并且根据碰到的那个位置来改变速度
		const GLfloat centerBoard = player->position.x + player->size.x / 2;
		const GLfloat distance = (ball->position.x + ball->radius) - centerBoard;
		const GLfloat percentage = distance / (player->size.x / 2);
		//根据结果来移动
		const GLfloat strength = 2.0f;
		const glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = BallObject::C_BallVelocity.x * percentage * strength;
		//防止内嵌
		ball->position.y = player->position.y + PlayerObject::C_PlayerSize.y;
		//如果速度过快 则可能位于下方  所以需要绝对确保
		ball->velocity.y = abs(ball->velocity.y);
		//保持总速度不变
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
	}
}

void Game::CheckFail()
{
	if (ball->position.y <= 0)
	{
		ReLoadLevel(level);
		ResetPlayer();
	}
}

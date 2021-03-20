#include "Game.h"

#include <windows.h>

#include "BallObject.h"
#include "Collider2D.h"
#include "DebugLog.h"
#include "PlayerObject.h"
#include "PowerUp.h"
#include "KeyInput.h"


const unsigned int Game::C_PlayerLife = 3;

/*
 * TODO:
 * 1.字体:放一个图片上,mesh combine
 * 2.砖块用drawinstance绘制 , 图片打成图集combine
 * 3.四边形的用四个顶点加顶点索引绘制  这将保存发送到 GPU 的三分之一数据。TRIANGLE_STRIP46
 * 4.空间区分计算碰撞
 * 5.减少状态切换 实现srp batcher
 * 6.打包 https://www.jianshu.com/p/0073859e9b0b
 */


Game::Game(GLuint _width, GLuint _height)
{
	width = _width;
	height = _height;
	mapSize = glm::vec2(_width, _height);
	const GLuint halfWidth = _width / 2;
	const GLuint halfHeight = _height / 2;

	camera.Init(glm::vec3(halfWidth, halfHeight, 0), glm::vec3(0, 0, 0), glm::vec3(halfWidth, halfHeight, 1));

	life = 0;
	state = GameState::GAME_MENU;
	this->level = 0;
}

Game::~Game()
{
	delete postProcessor;
	delete spriteRenderer;
	delete player;
	delete ball;
	delete particleGenerator;
	soundEngine->drop(); //已经delete了
	soundEngine = nullptr;
}

void Game::Init()
{
	InitRes();
	life = C_PlayerLife;
	state = GameState::GAME_MENU;
	//数组从0开始    关卡的名字从1开始
	this->level = 1;
	textRenderer = new TextRenderer(this->width, this->height,
	                                resourceManager.GetShader(ConstConfigure::Shader_TextKey));
	textRenderer->Load(ConstConfigure::Font_OcraextPath, 24);
	powerUpGenerator = new PowerUpGenerator{};
	soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->setSoundVolume(0); //禁音
	soundEngine->play2D(ConstConfigure::Audio_BreakoutPath.c_str(), GL_TRUE);
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
		resourceManager.GetShader(ConstConfigure::Shader_ParticleKey), camera.GetViewProjection()
		, resourceManager.GetTexture(ConstConfigure::Image_ParticleKey), 500);
}

void Game::InitRes()
{
	//load 这一块其实可以放到json/xml 里面做
	//Shader
	resourceManager.LoadShader(ConstConfigure::Shader_SpriteKey, ConstConfigure::Shader_SpritePath);
	resourceManager.LoadShader(ConstConfigure::Shader_ParticleKey, ConstConfigure::Shader_ParticlePath);
	resourceManager.LoadShader(ConstConfigure::Shader_PostProcessKey, ConstConfigure::Shader_PostProcessPath);
	resourceManager.LoadShader(ConstConfigure::Shader_TextKey, ConstConfigure::Shader_TextPath);

	//Texture
	resourceManager.LoadTexture(ConstConfigure::Image_BackgroundKey, ConstConfigure::Image_BackgroundPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockKey, ConstConfigure::Image_BlockPath);
	resourceManager.LoadTexture(ConstConfigure::Image_BlockSolidKey, ConstConfigure::Image_BlockSolidPath);
	resourceManager.LoadTexture(ConstConfigure::Image_PaddleKey, ConstConfigure::Image_PaddlePath);
	resourceManager.LoadTexture(ConstConfigure::Image_BallKey, ConstConfigure::Image_BallPath);
	resourceManager.LoadTexture(ConstConfigure::Image_ParticleKey, ConstConfigure::Image_ParticlePath);

	resourceManager.LoadTexture(ConstConfigure::Image_SpeedKey, ConstConfigure::Image_SpeedPath);
	resourceManager.LoadTexture(ConstConfigure::Image_StickyKey, ConstConfigure::Image_StickyPath);
	resourceManager.LoadTexture(ConstConfigure::Image_PassKey, ConstConfigure::Image_PassPath);
	resourceManager.LoadTexture(ConstConfigure::Image_PadKey, ConstConfigure::Image_PadPath);
	resourceManager.LoadTexture(ConstConfigure::Image_ConfuseKey, ConstConfigure::Image_ConfusePath);
	resourceManager.LoadTexture(ConstConfigure::Image_ChaosKey, ConstConfigure::Image_ChaosPath);


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

void Game::ResetPlayer()
{
	player->Reset();
	ball->Reset(player->position, player->size);
	postProcessor->Reset();
	powerUpGenerator->Reset();
	particleGenerator->Reset();
}

void Game::ProcessInput(GLfloat dt)
{
	keyInput.Update();


	if (this->state == GameState::GAME_MENU)
	{
		if (keyInput.IsPress(VK_RETURN))
		{
			this->state = GameState::GAME_ACTIVE;
		}
		else if (keyInput.IsPress('W'))
		{
			//有四关
			this->level = (this->level + 1) % 4;
		}
		else if (keyInput.IsPress('S'))
		{
			this->level = (this->level - 1) % 4;
		}
	}
	else if (this->state == GameState::GAME_ACTIVE)
	{
		GLfloat moveX = 0;
		GLfloat velocity = 0;
		if (keyInput.IsHold('A'))
		{
			velocity -= dt;
		}
		if (keyInput.IsHold('D'))
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
		if (keyInput.IsHold(VK_SPACE))
		{
			ball->stuck = false;
		}
	}
	else if (this->state == GameState::GAME_WIN)
	{
		if (keyInput.IsPress(VK_RETURN))
		{
			ResetPlayer();
			this->state = GameState::GAME_MENU;
			life = C_PlayerLife;
			ReLoadLevel(level);
		}
	}
}

void Game::Update(GLfloat dt)
{
	if (state == GameState::GAME_ACTIVE)
	{
		ball->Move(dt);
		particleGenerator->Update(dt, *ball, 2, glm::vec2(ball->radius / 2));
		this->CheckCollisions();
		powerUpGenerator->UpdatePowerUps(dt, this);
		postProcessor->Update(dt);
		CheckFail();
		CheckWin();
	}
}

void Game::Render()
{
	// glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	// glClearDepth(1.0);
	// glClear(GL_COLOR_BUFFER_BIT); //| GL_DEPTH_BUFFER_BIT);


	postProcessor->BeginRender();

	spriteRenderer->DrawSprite(resourceManager.GetTexture(ConstConfigure::Image_BackgroundKey)
	                           , glm::vec2(0, 0), glm::vec2(this->width, this->height), 0);
	this->levels[this->level].Draw(*spriteRenderer);

	this->powerUpGenerator->Draw(*spriteRenderer);

	player->Draw(*spriteRenderer);
	if (!ball->stuck)
	{
		particleGenerator->Draw();
	}
	ball->Draw(*spriteRenderer);

	postProcessor->EndRender();

	postProcessor->Render(static_cast<GLfloat>(glfwGetTime()));


	if (this->state == GameState::GAME_MENU)
	{
		textRenderer->RenderText("Press ENTER to start", 250.0f, this->height / 2.0f, 1.0f);
		textRenderer->RenderText("Press W or S to select level", 245.0f, this->height / 2 + 20.0f, 0.75f);
	}
	else if (this->state == GameState::GAME_ACTIVE)
	{
		textRenderer->RenderText("Life:" + std::to_string(this->life), 5.0f, 5.0f, 1.0f);
	}
	else if (this->state == GameState::GAME_WIN)
	{
		textRenderer->RenderText(
			"You WON!!!", 320.0, this->height / 2 - 20.0, 1.0, glm::vec4(0.0, 1.0, 0.0, 1.0)
		);
		textRenderer->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, this->height / 2, 1.0, glm::vec4(1.0, 1.0, 0.0, 1.0)
		);
	}
}

void Game::ActivatePowerUp(PowerUp& powerUp) const
{
	soundEngine->play2D(ConstConfigure::Audio_BreakoutPath.c_str());
	const auto itemType = powerUp.itemType;

	if (itemType == PowerUp::ItemType::Speed)
	{
		ball->velocity *= 1.2;
	}
	else if (itemType == PowerUp::ItemType::Sticky)
	{
		ball->sticky = true;
		player->color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (itemType == PowerUp::ItemType::Pass)
	{
		ball->passThrough = true;
		ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (itemType == PowerUp::ItemType::Pad)
	{
		player->size.x += 50;
	}
	else if (itemType == PowerUp::ItemType::Confuse)
	{
		if (!postProcessor->chaos)
		{
			postProcessor->confuse = true; // only activate if chaos wasn't already active
		}
	}
	else if (itemType == PowerUp::ItemType::Chaos)
	{
		if (!postProcessor->confuse)
		{
			postProcessor->chaos = true;
		}
	}
}

void Game::StopPowerUp(PowerUp& powerUp) const
{
	if (powerUp.itemType == PowerUp::ItemType::Sticky)
	{
		ball->sticky = false;
		player->color = glm::vec3(1.0f);
	}
	else if (powerUp.itemType == PowerUp::ItemType::Pass)
	{
		ball->passThrough = false;
		ball->color = glm::vec3(1.0f);
	}
	else if (powerUp.itemType == PowerUp::ItemType::Confuse)
	{
		postProcessor->confuse = false; // only activate if chaos wasn't already active
	}
	else if (powerUp.itemType == PowerUp::ItemType::Chaos)
	{
		postProcessor->chaos = false;
	}
}

void Game::CheckCollisions()
{
	auto&& level = this->levels[this->level];
	for (GameObject& tile : level.bricks)
	{
		if (!tile.destroyed)
		{
			CollisionInfo collisionInfo = Collider2D::CheckBallCollision(*ball, tile);
			if (std::get<0>(collisionInfo))
			{
				if (!tile.isSolid)
				{
					tile.destroyed = GL_TRUE;
					soundEngine->play2D(ConstConfigure::Audio_BleepMp3Path.c_str());
					powerUpGenerator->SpawnPowerUps(&resourceManager, tile);
					level.breakCount--;
				}
				else
				{
					soundEngine->play2D(ConstConfigure::Audio_SolidPath.c_str());
					postProcessor->DoShake(0.05f);
				}

				if (!(!tile.isSolid && ball->passThrough))
				{
					//碰撞处理
					Rigibody2D::Direction dir = std::get<1>(collisionInfo);
					const glm::vec2 diff_vector = std::get<2>(collisionInfo);
					if (dir == Rigibody2D::Direction::LEFT || dir == Rigibody2D::Direction::RIGHT)
					{
						ball->velocity.x = -ball->velocity.x; //反转水平速度
						//重新定位  先找出内嵌多少距离
						const GLfloat penetration = ball->radius - std::abs(diff_vector.x);
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
	}
	CollisionInfo result = Collider2D::CheckBallCollision(*ball, *player);
	if (!ball->stuck && std::get<0>(result))
	{
		soundEngine->play2D(ConstConfigure::Audio_BleepWavPath.c_str());

		//防止内嵌
		ball->position.y = player->position.y + PlayerObject::C_PlayerSize.y;

		if (ball->sticky)
		{
			ball->stuck = true;
			ball->velocity = ball->C_BallVelocity;
		}
		else
		{
			//检测碰到了挡板的哪个位置,并且根据碰到的那个位置来改变速度
			const GLfloat centerBoard = player->position.x + player->size.x / 2;
			const GLfloat distance = (ball->position.x + ball->radius) - centerBoard;
			const GLfloat percentage = distance / (player->size.x / 2);
			//根据结果来移动
			const GLfloat strength = 2.0f;
			const glm::vec2 oldVelocity = ball->velocity;
			ball->velocity.x = BallObject::C_BallVelocity.x * percentage * strength;
			//如果速度过快 则可能位于下方  所以需要绝对确保
			ball->velocity.y = abs(ball->velocity.y);
			//保持总速度不变
			ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		}
	}
}

void Game::CheckFail()
{
	if (ball->position.y <= 0)
	{
		this->life--;
		ResetPlayer();

		if (this->life <= 0)
		{
			this->state = GameState::GAME_MENU;
			life = C_PlayerLife;
			ReLoadLevel(level);
		}
	}
}

void Game::CheckWin()
{
	if (this->levels[this->level].breakCount <= 0)
	{
		postProcessor->chaos = true;
		this->state = GameState::GAME_WIN;
	}
}

#include "Program.h"

#include <iostream>

Program* Program::instance = nullptr;

Program::~Program()
{
	instance = nullptr;
}

Program::Program(GLuint width, GLuint height): breakout(width, height)
{
	instance = this;
	screen_width = width;
	screen_height = height;
	deltaTime = 0;
	lastFrame = 0;
}

bool Program::Init(bool canResize, bool canDebug)
{
	const int ret = glfwInit();
	if (!ret)
	{
		std::cout << "Failed to glfwInit.\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用OpenGL核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //苹果显示需要
#endif
	glfwWindowHint(GLFW_RESIZABLE, canResize);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, canDebug);

	return true;
}

bool Program::CreateWindow()
{
	window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //设置回调函数，改变窗口大小

	//初始化glad ,把OpenGL的函数指针导入给GLAD ,但是要在窗口初始化之后执行
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//glGetError();//glewInit()才需要

	return true;
}

bool Program::RegisterEvent()
{
	glfwSetKeyCallback(window, KeyCallBack);
	glViewport(0, 0, screen_width, screen_height);
	return true;
}

bool Program::PreLoop()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	breakout.Init();

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	breakout.State = GameState::GAME_ACTIVE;
	return true;
}

bool Program::DoLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		const GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		breakout.ProcessInput(deltaTime);
		breakout.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		breakout.Render();

		glfwSwapBuffers(window);
	}
	return true;
}

bool Program::EndLoop()
{
	resourceManager.Clear();
	glfwTerminate();
	return true;
}

void Program::DoMain()
{
	if (!Init())
	{
		return;
	}

	if (!CreateWindow())
	{
		return;
	}

	if (!RegisterEvent())
	{
		return;
	}

	if (!PreLoop())
	{
		return;
	}

	if (!DoLoop())
	{
		return;
	}

	if (!EndLoop())
	{
		return;
	}
}


//当用户改变窗口的大小的时候，视口也应该被调整的回调
void Program::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //设置窗口的维度
}


void Program::KeyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			instance->breakout.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			instance->breakout.Keys[key] = GL_FALSE;
	}
}

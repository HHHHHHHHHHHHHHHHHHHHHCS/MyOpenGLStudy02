#include <iostream>
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的 请确保GLAD头文件的引入在GLFW之前
#include <GLFW/glfw3.h>

#include "Program.h"
#include "Game.h"
#include "ResourceManager.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);


//TODO:移动到Program
int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用OpenGL核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //苹果显示需要
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	Program program{};

	GLFWwindow* window = glfwCreateWindow(program.screen_width, program.screen_height, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //设置回调函数，改变窗口大小
	//初始化glad ,把OpenGL的函数指针导入给GLAD ,但是要在窗口初始化之后执行
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glfwInit();
	//glGetError();//glewInit()才需要

	if (window == nullptr)
	{
		return -1;
	}


	glfwSetKeyCallback(window, Program::KeyCallBack);
	glViewport(0, 0, program.screen_width, program.screen_height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game game = program.breakout;

	game.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	game.State = GameState::GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		game.ProcessInput(deltaTime);
		game.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		game.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();
	glfwTerminate();

	return 0;
}

//当用户改变窗口的大小的时候，视口也应该被调整的回调
//TODO移动到program
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //设置窗口的维度
}
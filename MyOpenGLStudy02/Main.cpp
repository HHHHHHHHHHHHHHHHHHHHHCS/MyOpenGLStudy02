#include <iostream>
#include <glad/glad.h>//GLAD����������OpenGL�ĺ���ָ��� ��ȷ��GLADͷ�ļ���������GLFW֮ǰ
#include <GLFW/glfw3.h>

#include "Program.h"
#include "Game.h"
#include "ResourceManager.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);


//TODO:�ƶ���Program
int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ��OpenGL����ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //ƻ����ʾ��Ҫ
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	Program program{};

	GLFWwindow* window = glfwCreateWindow(program.screen_width, program.screen_height, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //���ûص��������ı䴰�ڴ�С
	//��ʼ��glad ,��OpenGL�ĺ���ָ�뵼���GLAD ,����Ҫ�ڴ��ڳ�ʼ��֮��ִ��
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glfwInit();
	//glGetError();//glewInit()����Ҫ

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

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ������Ļص�
//TODO�ƶ���program
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //���ô��ڵ�ά��
}
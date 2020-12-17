#pragma once
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的 请确保GLAD头文件的引入在GLFW之前
#include <GLFW/glfw3.h>

#include "Game.h"

class Program
{
public:
	static Program* instance;

	Game game;


	Program(GLuint width = 800, GLuint height = 600);
	~Program();

	void DoMain();

private:
	GLFWwindow* window = nullptr;

	GLuint screen_width;
	GLuint screen_height;
	GLfloat deltaTime;
	GLfloat lastFrame;

	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void KeyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mode);

	bool Init(bool canResize = false, bool canDebug = true) ;
	bool CreateWindow();
	bool RegisterEvent() ;
	bool PreLoop();
	bool DoLoop();
	bool EndLoop();
};

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"

class Program
{
public:
	static Program* instance;

	~Program();
	
	GLuint screen_width;
	GLuint screen_height;

	Game breakout;

	Program(GLuint width = 800, GLuint height = 600);

	static void KeyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mode);
};

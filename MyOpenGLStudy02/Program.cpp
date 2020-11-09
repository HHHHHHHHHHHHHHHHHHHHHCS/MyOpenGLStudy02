#include "Program.h"

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

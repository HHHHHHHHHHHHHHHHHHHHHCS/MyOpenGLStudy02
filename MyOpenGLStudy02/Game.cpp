#include "Game.h"

#include <glad/glad.h>//GLAD����������OpenGL�ĺ���ָ��� ��ȷ��GLADͷ�ļ���������GLFW֮ǰ
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ��OpenGL����ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //ƻ����ʾ��Ҫ
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);


}

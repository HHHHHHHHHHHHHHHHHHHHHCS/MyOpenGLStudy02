#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vec4.hpp>
#include <ext/matrix_float4x4.hpp>

class DebugLog
{
public:
	static void Print( GLfloat val);
	static void Print(const glm::vec2& val);
	static void Print(const glm::vec3& val);
	static void Print(const glm::vec4& val);
	static void Print(const glm::mat4& val);
};

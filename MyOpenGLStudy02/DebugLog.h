#pragma once
#include <vec4.hpp>
#include <ext/matrix_float4x4.hpp>

class DebugLog
{
public:
	static void Print(const glm::vec3& val);
	static void Print(const glm::vec4& val);
	static void Print(const glm::mat4& val);
};

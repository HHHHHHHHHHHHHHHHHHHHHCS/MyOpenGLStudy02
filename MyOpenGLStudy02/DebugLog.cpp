#include "DebugLog.h"

#include <iostream>

void DebugLog::Print(std::string val)
{
	printf("%s  \n", val.c_str());
}

void DebugLog::Print(GLfloat val)
{
	printf("%f  \n", val);
}

void DebugLog::Print(const glm::vec2& val)
{
	printf("x: %f , y: %f \n", val.x, val.y);
}

void DebugLog::Print(const glm::vec3& val)
{
	printf("x: %f , y: %f , z: %f \n", val.x, val.y, val.z);
}

void DebugLog::Print(const glm::vec4& v4)
{
	printf("x: %f , y: %f , z: %f , w: %f \n", v4.x, v4.y, v4.z, v4.w);
}

void DebugLog::Print(const glm::mat4& m4)
{
	printf("%f | %f | %f | %f \n", m4[0].x, m4[1].x, m4[2].x, m4[3].x);
	printf("%f | %f | %f | %f \n", m4[0].y, m4[1].y, m4[2].y, m4[3].y);
	printf("%f | %f | %f | %f \n", m4[0].z, m4[1].z, m4[2].z, m4[3].z);
	printf("%f | %f | %f | %f \n", m4[0].w, m4[1].w, m4[2].w, m4[3].w);
}

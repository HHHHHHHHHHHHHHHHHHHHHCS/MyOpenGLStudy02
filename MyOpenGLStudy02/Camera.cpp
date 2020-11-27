#include "Camera.h"

#include <iostream>

void Camera::Init(glm::vec3 pos, glm::vec3 rot, glm::vec3 size)
{
	SetPosition(pos);
	SetRotation(rot);
	SetSize(size);
}

glm::vec3 Camera::GetPosition() const
{
	return this->position;
}

void Camera::SetPosition(glm::vec3 val)
{
	this->position = val;
	posFlag = true;
}

glm::vec3 Camera::GetRotation() const
{
	return this->rotation;
}

void Camera::SetRotation(glm::vec3 val)
{
	this->rotation = val;
	rotFlag = true;
}

glm::vec3 Camera::GetSize() const
{
	return this->size;
}

void Camera::SetSize(glm::vec3 val)
{
	this->size = val;
	sizeFlag = true;
}

glm::mat4 Camera::GetViewMat4()
{
	if (!posFlag && !rotFlag)
	{
		return viewMat4;
	}
	posFlag = false;
	rotFlag = false;

	viewMat4 = glm::mat4(1.0f);
	viewMat4 = glm::translate(viewMat4, position);
	viewMat4 = glm::rotate(viewMat4, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	viewMat4 = glm::rotate(viewMat4, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	viewMat4 = glm::rotate(viewMat4, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	
	//摄像机空间 要翻转左右手坐标系   Z相反
    viewMat4[0].z = -viewMat4[0].z;
    viewMat4[1].z = -viewMat4[1].z;
    viewMat4[2].z = -viewMat4[2].z;
    viewMat4[3].z = -viewMat4[3].z;
	
	viewMat4 = inverse(viewMat4);
	
	return viewMat4;
}


glm::mat4 Camera::GetProjectionMat4()
{
	if (!sizeFlag)
	{
		return projMat4;
	}

	sizeFlag = false;

	float x = size.x;
	float y = size.y;
	float z = size.z;

	//opengl y是相反的
	projMat4 = glm::ortho(-x, +x, -y, +y, -z, +z);

	return projMat4;
}

glm::mat4 Camera::GetViewProjection()
{
	if (!posFlag && !rotFlag && !sizeFlag)
	{
		return viewProjMat4;
	}

	viewProjMat4 = GetProjectionMat4() * GetViewMat4();

	return viewProjMat4;
}

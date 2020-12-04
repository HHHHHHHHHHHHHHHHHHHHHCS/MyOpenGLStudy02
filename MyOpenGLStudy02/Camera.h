#pragma once
#include <vec3.hpp>
#include <ext/matrix_float4x4.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

class Camera
{
public:
	void Init(const glm::vec3& pos = glm::vec3(400, 300, 0), const glm::vec3& rot = glm::vec3(0, 0, 0),
	          const glm::vec3& size = glm::vec3(800, 600, 1));

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& val);
	glm::vec3 GetRotation() const;
	void SetRotation(const glm::vec3& val);
	glm::vec3 GetSize() const;
	void SetSize(const glm::vec3& val);

	glm::mat4 GetViewMat4();
	glm::mat4 GetProjectionMat4();
	glm::mat4 GetViewProjection();

private:
	bool posFlag;
	bool rotFlag;
	bool sizeFlag;


	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;

	glm::mat4 viewMat4;
	glm::mat4 projMat4;
	glm::mat4 viewProjMat4;
};

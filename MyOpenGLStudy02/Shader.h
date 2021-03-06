﻿#pragma once
#include <string>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint ID = 0;

	//空的 后面传入编译才进行Init
	Shader();

	Shader& Use();
	bool Compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar* geometryCode = nullptr);
	//Utility functions
	void SetFloat(const GLchar* name, GLfloat value, bool useShader = false);
	void SetInteger(const GLchar* name, GLint value, bool useShader = false);
	void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, bool useShader = false);
	void SetVector2f(const GLchar* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, bool useShader = false);
	void SetVector3f(const GLchar* name, const glm::vec3& value, bool useShader = false);
	void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, bool z, GLfloat w, GLboolean useShader = false);
	void SetVector4f(const GLchar* name, const glm::vec4& value, bool useShader = false);
	void SetMatrix4x4(const GLchar* name, const glm::mat4& matrix, bool useShader = false);
	void SetVector1iArray(const GLchar* name, GLint length, const GLint* value, bool useShader = false);
	void SetVector1fArray(const GLchar* name, GLint length, const GLfloat* value, bool useShader = false);
	void SetVector2fArray(const GLchar* name, GLint length, const GLfloat* value, bool useShader = false);
private:
	//检查编译错误
	bool CheckCompileErrors(GLuint object, std::string type);
};

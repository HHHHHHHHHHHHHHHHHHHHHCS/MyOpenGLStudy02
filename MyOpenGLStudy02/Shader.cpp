﻿#include "Shader.h"

#include <iostream>

Shader::Shader()
{
}

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

bool Shader::Compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar* geometryCode)
{
	GLuint vertexShader, fragmentShader, geometryShader;

	bool result = true;

	//Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	if (!CheckCompileErrors(vertexShader, "VERTEX"))
	{
		result = false;
	}

	//Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	if (!CheckCompileErrors(fragmentShader, "FRAGMENT"))
	{
		result = false;
	}
	//Geometry Shader
	if (geometryCode != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryCode, NULL);
		glCompileShader(geometryShader);
		if (!CheckCompileErrors(geometryShader, "GEOMETRY"))
		{
			result = false;
		}
	}
	//Shader program
	this->ID = glCreateProgram();
	glUseProgram(this->ID);
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	if (geometryCode != nullptr)
	{
		glAttachShader(this->ID, geometryShader);
	}
	glLinkProgram(this->ID);
	if (!CheckCompileErrors(this->ID, "PROGRAM"))
	{
		result = false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryCode != nullptr)
	{
		glDeleteShader(geometryShader);
	}

	return result;
}

void Shader::SetFloat(const GLchar* name, GLfloat value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const GLchar* name, GLint value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f(const GLchar* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	//glUniform2fv(glGetUniformLocation(this->ID, name), 1, &value[0]);
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, bool z, GLfloat w, GLboolean useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4x4(const GLchar* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1,GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVector1iArray(const GLchar* name, const GLint length, const GLint* value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1iv(glGetUniformLocation(this->ID, name), length, value);
}

void Shader::SetVector1fArray(const GLchar* name, const GLint length, const GLfloat* value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1fv(glGetUniformLocation(this->ID, name), length, value);
}

void Shader::SetVector2fArray(const GLchar* name, const GLint length, const GLfloat* value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2fv(glGetUniformLocation(this->ID, name), length, value);
}

bool Shader::CheckCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM") //type == "SHADER"
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR:: SHADER: Compile-time error:Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
			return false;
		}
	}
	else // if(type == "PROGRAM")
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
			return false;
		}
	}
	return true;
}

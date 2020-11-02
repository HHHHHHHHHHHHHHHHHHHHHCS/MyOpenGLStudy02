#include "Shader.h"

Shader::Shader()
{
}

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	GLuint vertexShader, fragmentShader, geometryShader;

	//Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CheckCompileErrors(vertexShader, "VERTEX");
	//Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CheckCompileErrors(fragmentShader, "FRAGMENT");
	//Geometry Shader
	if (geometrySource != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		glCompileShader(geometryShader);
		CheckCompileErrors(geometryShader, "GEOMETRY");
	}
	//Shader program
	this->ID = glCreateProgram();
	glUseProgram(this->ID);
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	if (geometrySource != nullptr)
	{
		glAttachShader(this->ID, geometryShader);
	}
	glLinkProgram(this->ID);
	CheckCompileErrors(this->ID, "PROGRAM");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometrySource != nullptr)
	{
		glDeleteShader(geometryShader);
	}
}


void Shader::Compile(const std::string& fileSource, const bool& haveGeometry)
{
	this->Compile((fileSource + ".vs").c_str(), (fileSource + ".fs").c_str(),
	              haveGeometry == true ? (fileSource + ".gs").c_str() : nullptr);
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
	glUniform2fv(glGetUniformLocation(this->ID, name), 1, &value[0]);
}

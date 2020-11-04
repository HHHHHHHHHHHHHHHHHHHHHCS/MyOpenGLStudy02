#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile,
                                   std::string name)
{
	shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}

Shader ResourceManager::LoadShader(const std::string& shaderFile, std::string name, const GLboolean haveGeometry)
{
	shaders[name] = LoadShaderFromFile(shaderFile, haveGeometry);
	return shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
	textures[name] = LoadTextureFromFile(file, alpha);
	return textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
	return textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : shaders)
	{
		glDeleteProgram(iter.second.ID);
	}

	for (auto iter : textures)
	{
		glDeleteTextures(1, &iter.second.ID);
	}
}

Shader ResourceManager::LoadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile,
                                           const GLchar* gShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try
	{
		//file stream
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		//string stream
		std::stringstream vShaderStream, fShaderStream;
		//load string to string stream from file stream
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//close file stream
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//convert stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER:Failed to read shader files!" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);

	return shader;
}

Shader ResourceManager::LoadShaderFromFile(const std::string& shaderFile, const GLboolean haveGeometry)
{
	return LoadShaderFromFile((shaderFile + ".vs").c_str(), (shaderFile + ".fs").c_str()
	                          , haveGeometry ? (shaderFile + ".gs").c_str() : nullptr);
}

#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <iostream>
#include <sstream>

ResourceManager::ResourceManager()
{
}


Shader ResourceManager::LoadShader(const std::string& name, const GLchar* vShaderFile, const GLchar* fShaderFile,
                                   const GLchar* gShaderFile, const std::string& directory)
{
	shaders[name] = LoadShaderFromFile((directory + vShaderFile).c_str()
	                                   , (directory + fShaderFile).c_str(),
	                                   gShaderFile == nullptr ? nullptr : (directory + gShaderFile).c_str());
	return shaders[name];
}

Shader ResourceManager::LoadShader(const std::string& name, const std::string& shaderFile,
                                   const std::string& directory, const GLboolean haveGeometry)
{
	shaders[name] = LoadShaderFromFile(directory + shaderFile, haveGeometry);
	return shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return shaders[name];
}

Texture2D ResourceManager::LoadTexture(const std::string& name, const GLchar* file, const std::string& directory)
{
	textures[name] = LoadTextureFromFile((directory + file).c_str());
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

Texture2D ResourceManager::LoadTextureFromFile(const GLchar* file, const GLenum isSRGB)
{
	Texture2D texture;


	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;

		if (isSRGB)
		{
			switch (nrChannels)
			{
			case 1:
				internalFormat = dataFormat = GL_RED;
				break;
			case 2:
				internalFormat = dataFormat = GL_RG;
				break;
			case 3:
				internalFormat = GL_SRGB;
				dataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_SRGB_ALPHA;
				dataFormat = GL_RGBA;
				break;
			}
		}
		else
		{
			switch (nrChannels)
			{
			case 1:
				internalFormat = dataFormat = GL_RED;
				break;
			case 2:
				internalFormat = dataFormat = GL_RG;
				break;
			case 3:
				internalFormat = dataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = dataFormat = GL_RGBA;
				break;
			}
		}

		Texture2D texture;
		texture.Internal_Format = internalFormat;
		texture.Image_Format = dataFormat;
		texture.Generate(width, height, data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << file << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

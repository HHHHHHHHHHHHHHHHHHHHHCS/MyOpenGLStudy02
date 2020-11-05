#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb-master/stb_image.h"

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

Texture2D ResourceManager::LoadTextureFromFile(const std::string& file, const std::string& directory,
                                               const GLenum isSRGB)
{
	Texture2D texture;

	int width, height, nrChannels;
	unsigned char* data = stbi_load((directory + file).c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		//todo:
		
	if(data)
	{
		GLenum internalFormat;
		GLenum dataFormat;

		if(isSRGB)
		{
			switch (nrChannels)
			{
			case  1:
				internalFormat = dataFormat = GL_RED;
				break;
			case  2:
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
			case  1:
				internalFormat = dataFormat = GL_RED;
				break;
			case  2:
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

		//TODO:

				//第一个是 纹理类型 1D  2D  3D
		//第二个是 当前图片的mipmap层级,可以自动生成,也可以手动修改层级 设置图片
		//第三个是 颜色类型 gamma(srgb) linear
		//第四个是 纹理的宽度
		//第五个是 纹理的高度
		//第六个是 总是为0 历史遗留问题
		//第七个是 图片颜色格式 R RG RGB RGBA
		//第八个是 图片单个颜色的数据类型
		//第九个是 图片的数据
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		//自动生成mipmap
		glGenerateMipmap(GL_TEXTURE_2D);


		// 为当前绑定的纹理对象设置环绕、过滤方式
		// S->X T->Y W->Z 如果是3D 的则有W
		//GL_TEXTURE_WRAP 是重复方式  GL_TEXTURE_???_FILTER 是放大缩小的滤波方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

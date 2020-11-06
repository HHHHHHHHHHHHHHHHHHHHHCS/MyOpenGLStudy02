#pragma once
#include <map>

#include "Shader.h"
#include "Texture2D.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader LoadShader(const std::string& name, const GLchar* vShaderFile, const GLchar* fShaderFile,
	                         const GLchar* gShaderFile = nullptr, const std::string& directory = "Shader/");

	static Shader LoadShader(const std::string& name, const std::string& shaderFile,
	                         const std::string& directory = "Shader/", const GLboolean haveGeometry = false);

	static Shader GetShader(const std::string& name);

	static Texture2D LoadTexture(const std::string& name, const GLchar* file, const std::string& directory = "Image/");

	static Texture2D GetTexture(const std::string& name);

	static void Clear();

private:
	ResourceManager();

	static Shader LoadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile);

	static Shader LoadShaderFromFile(const std::string& shaderFile, const GLboolean haveGeometry = false);

	static Texture2D LoadTextureFromFile(const GLchar* file, const GLenum isSRGB = false);
};

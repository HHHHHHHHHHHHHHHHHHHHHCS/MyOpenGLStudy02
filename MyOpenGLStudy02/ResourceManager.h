#pragma once
#include <map>


#include "ConstConfigure.h"
#include "Shader.h"
#include "Texture2D.h"

class ResourceManager
{
public:
	std::map<std::string, Shader> shaders;
	std::map<std::string, Texture2D> textures;

	ResourceManager();

	Shader LoadShader(const std::string& name, const GLchar* vShaderFile, const GLchar* fShaderFile,
	                  const GLchar* gShaderFile = nullptr,
	                  const std::string& directory = ConstConfigure::Shader_Directory);

	Shader LoadShader(const std::string& name, const std::string& shaderFile,
	                  const std::string& directory = ConstConfigure::Shader_Directory, GLboolean haveGeometry = false);

	Shader GetShader(const std::string& name);

	Texture2D* LoadTexture(const std::string& name, const std::string& file,
	                       const std::string& directory = ConstConfigure::Image_Directory);

	Texture2D* GetTexture(const std::string& name);

	void Clear();

private:

	Shader LoadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile);

	Shader LoadShaderFromFile(const std::string& shaderFile, GLboolean haveGeometry = false);

	Texture2D LoadTextureFromFile(const GLchar* file, GLenum isSRGB = false);
};

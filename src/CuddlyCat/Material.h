#pragma once

#include "define.h"
#include "Shader.h"
using namespace std;

NS_CC_BEGIN



class Material
{
public:
	Material();
	~Material();

	void setShader(const std::string& vertexPath, const std::string& fragmentPath);
	void setDiffuseTexture(unsigned int id) 
	{
		_diffuseTexture = id;
	}
	void setSpecularTexture(unsigned int id)
	{
		_specularTexture = id;
	}
	void setNormalTexture(unsigned int id)
	{
		_normalTexture = id;
	}
	void setHeightTexture(unsigned int id)
	{
		_heightTexture = id;
	}

private:
	Shader* _shader;

	unsigned int _diffuseTexture;
	unsigned int _specularTexture;
	unsigned int _normalTexture;
	unsigned int _heightTexture;
};





NS_CC_END
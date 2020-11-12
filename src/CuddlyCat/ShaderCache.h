#pragma once
#include "define.h"
#include "ShaderCache.h"
#include <string>
#include <vector>
#include "Shader.h"


NS_CC_BEGIN

class ShaderCache
{
public:
	static ShaderCache* getInstance();
	static void destroyInstance();

	ShaderCache();
	~ShaderCache();

	bool init();

	Shader* loadByFile(const std::string& vertexPath, const std::string& fragmentPath);

private:
	std::vector<Shader*> _shaders;
};

NS_CC_END
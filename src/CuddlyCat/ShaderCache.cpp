#include "ShaderCache.h"

NS_CC_BEGIN

static ShaderCache *s_SharedShaderCache = nullptr;

ShaderCache* ShaderCache::getInstance()
{
	if (!s_SharedShaderCache)
	{
		s_SharedShaderCache = new ShaderCache;
		s_SharedShaderCache->init();
	}

	return s_SharedShaderCache;
}

void ShaderCache::destroyInstance()
{
	if (s_SharedShaderCache != nullptr)
	{
		delete s_SharedShaderCache;
		s_SharedShaderCache = nullptr;
	}
}

ShaderCache::ShaderCache()
{
}

ShaderCache::~ShaderCache()
{
	for (int i = 0; i < _shaders.size(); i++) {
		delete _shaders[i];
	}
	_shaders.clear();
}

bool ShaderCache::init()
{
	return true;
}

Shader* ShaderCache::loadByFile(const std::string& vertexPath, const std::string& fragmentPath)
{
	for (int i = 0; i < _shaders.size(); i++) {
		if (_shaders[i]->getVertexPath() == vertexPath && _shaders[i]->getFragmentPath() == fragmentPath) {
			return _shaders[i];
		}
	}

	Shader* shader = new Shader;
	auto id = shader->initByFile(vertexPath, fragmentPath);
	_shaders.push_back(shader);
	return shader;
}


NS_CC_END



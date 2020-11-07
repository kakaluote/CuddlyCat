#include "Material.h"
#include "ShaderCache.h"

NS_CC_BEGIN

Material::Material()
{
}


Material::~Material()
{

}

void Material::setShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	_shader = ShaderCache::getInstance()->loadByFile(vertexPath, fragmentPath);
}

NS_CC_END


#include "Material.h"
#include "ShaderCache.h"
#include "TextureCache.h"

NS_CC_BEGIN

Material::Material():_shader(nullptr)
{
}


Material::~Material()
{
	_shader = nullptr;
	_textures.clear();
}

void Material::setShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	_shader = ShaderCache::getInstance()->loadByFile(vertexPath, fragmentPath);
}

void Material::addTexture(const std::string& path, const std::string& name)
{
	Texture2D* texture = TextureCache::getInstance()->loadByFile(path);
	texture->setName(name);
	_textures.push_back(texture);
}

void Material::use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
	_shader->use();

	_shader->setMat4("projection", projection);
	_shader->setMat4("view", view);
	_shader->setMat4("model", model);

	_shader->setVec3("lightPos", lightPos);
	_shader->setVec3("viewPos", viewPos);
	_shader->setVec3("lightColor", lightColor);

	for (int i = 0; i < _textures.size(); i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures[i]->id());
		_shader->setInt(_textures[i]->getName().c_str(), i);
	}
}

void Material::end()
{
	glActiveTexture(GL_TEXTURE0);
}

NS_CC_END


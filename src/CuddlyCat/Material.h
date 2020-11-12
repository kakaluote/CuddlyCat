#pragma once

#include "define.h"
#include "Shader.h"
#include <vector>
#include "Texture2D.h"
#include "glm/glm.hpp"
#include <string>
using namespace std;

NS_CC_BEGIN

class Texture2D;


class Material
{
public:
	Material();
	~Material();

	void setShader(const std::string& vertexPath, const std::string& fragmentPath);

	void addTexture(const std::string & path, const std::string& name);

	void use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::vec3& lightPos, const glm::vec3& lightColor);
	void end();

private:
	Shader* _shader;
	std::vector<Texture2D*> _textures;
};





NS_CC_END
#pragma once

#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "const.h"
#include <string>

NS_CC_BEGIN

class Texture
{
public:
	Texture();
	~Texture();

	unsigned int initByFile(const std::string& path);

	unsigned int id() const {
		return _textureID;
	}

	const std::string& getPath() const {
		return _path;
	}

private:
	unsigned int _textureID;
	std::string _path;
};

NS_CC_END
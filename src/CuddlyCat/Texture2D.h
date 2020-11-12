#pragma once

#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "const.h"
#include <string>
#include "ObjLoader.h"

NS_CC_BEGIN

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	unsigned int initByFile(const std::string& path);

	unsigned int id() const {
		return _textureID;
	}

	const std::string& getPath() const 
	{
		return _path;
	}

	void use(int idx);

	const std::string& getName() const
	{
		return _name;
	}
	void setName(const std::string& name)
	{
		_name = name;
	}

private:
	unsigned int _textureID;
	std::string _path;
	std::string _name;
};

NS_CC_END
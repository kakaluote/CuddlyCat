#include "Texture2D.h"
#include "CuddlyCat/stb_image.h"
#include "glad/gl.h"
#include <iostream>

NS_CC_BEGIN


Texture2D::Texture2D():_textureID(0)
{
}

Texture2D::~Texture2D()
{
	if (_textureID != 0) {
		GL_CHECK(glDeleteTextures(1, &_textureID));
	}
}

unsigned int Texture2D::initByFile(const std::string& path)
{
	_path = path;

	glGenTextures(1, &_textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);

	return _textureID;
}


void Texture2D::use(int idx)
{
	glActiveTexture(GL_TEXTURE0 + idx);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

NS_CC_END





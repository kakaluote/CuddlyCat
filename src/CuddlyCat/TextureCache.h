#pragma once
#include "define.h"
#include "TextureCache.h"
#include <string>
#include <vector>
#include "Texture.h"


NS_CC_BEGIN

class TextureCache
{
public:
	static TextureCache* getInstance();
	static void destroyInstance();

	TextureCache();
	~TextureCache();

	bool init();

	unsigned int loadByFile(const std::string& path);

private:
	std::vector<Texture*> _textures;
};

NS_CC_END
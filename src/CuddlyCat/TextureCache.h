#pragma once
#include "define.h"
#include "TextureCache.h"
#include <string>
#include <vector>
#include "Texture2D.h"


NS_CC_BEGIN

class TextureCache
{
public:
	static TextureCache* getInstance();
	static void destroyInstance();

	TextureCache();
	~TextureCache();

	bool init();

	Texture2D* loadByFile(const std::string& path);

private:
	std::vector<Texture2D*> _textures;
};

NS_CC_END
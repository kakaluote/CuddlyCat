#include "TextureCache.h"

NS_CC_BEGIN

static TextureCache *s_SharedTextureCache = nullptr;

TextureCache* TextureCache::getInstance()
{
	if (!s_SharedTextureCache)
	{
		s_SharedTextureCache = new TextureCache;
		s_SharedTextureCache->init();
	}

	return s_SharedTextureCache;
}

void TextureCache::destroyInstance()
{
	if (s_SharedTextureCache != nullptr)
	{
		delete s_SharedTextureCache;
		s_SharedTextureCache = nullptr;
	}
}

TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{
	for (int i = 0; i < _textures.size(); i++) {
		delete _textures[i];
	}
	_textures.clear();
}

bool TextureCache::init()
{
	return true;
}

unsigned int TextureCache::loadByFile(const std::string& path)
{
	for (int i = 0; i < _textures.size(); i++) {
		if (_textures[i]->getPath() == path) {
			return _textures[i]->id();
		}
	}

	Texture* texture = new Texture;
	auto id = texture->initByFile(path);
	_textures.push_back(texture);
	return id;
}


NS_CC_END



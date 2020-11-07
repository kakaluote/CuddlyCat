#pragma once

#include "define.h"
#include <string>
#include "assimp/scene.h"

NS_CC_BEGIN



class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static void loadModel(std::string const &path);
private:
	static void processNode(aiNode *node, const aiScene *scene);
	static void processMesh(aiMesh *mesh, const aiScene *scene);
	static vector<TextureInfo> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
};





NS_CC_END
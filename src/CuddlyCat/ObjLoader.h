#pragma once

#include "define.h"
#include <string>
#include "assimp/scene.h"
#include "glm/glm.hpp"
#include <vector>
#include "Material.h"
#include "Mesh.h"
#include "Node.h"

NS_CC_BEGIN


struct RawVertexData {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


struct RawTextureData {
	enum class Usage {
		Unknown = 0,
		Diffuse,
		Emissive,
		Ambient,
		Specular,
		Shininess,
		Normal,
		Bump,
		Transparency,
		Reflection,
		Height
	};
	Usage usage;
	std::string path;
	int idx;
	RawTextureData() :usage(Usage::Unknown), idx(0)
	{}
};

class Node;
class Material;
class Mesh;

class ObjLoader
{
public:
	static ObjLoader* getInstance();
	static void destroyInstance();

	ObjLoader();
	~ObjLoader();

	Node* loadModel(const std::string& path);
private:
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene, Mesh* m);
	void processMaterial(aiMesh * mesh, const aiScene *scene, Material* m);
	void loadMaterialTextures(aiMaterial *mat, aiTextureType type, Material* m);

private:
};





NS_CC_END
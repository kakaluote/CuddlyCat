#include "ObjLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include "Timer.h"
#include <vector>
#include "Mesh.h"
#include "Material.h"

using namespace std;

NS_CC_BEGIN

static ObjLoader *s_SharedObjLoader = nullptr;

ObjLoader* ObjLoader::getInstance()
{
	if (!s_SharedObjLoader)
	{
		s_SharedObjLoader = new ObjLoader;
	}

	return s_SharedObjLoader;
}

void ObjLoader::destroyInstance()
{
	CC_SAFE_DELETE(s_SharedObjLoader);
}


ObjLoader::ObjLoader()
{
}

ObjLoader::~ObjLoader()
{

}

Node* ObjLoader::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return nullptr;
	}

	Node* node = new Node;

	processNode(scene->mRootNode, scene);

	return node;
}

void ObjLoader::processNode(aiNode * node, const aiScene * scene)
{
	Timer t;
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		t.tik();
		

		Mesh* m = new Mesh;
		Material* mat = new Material;

		processMesh(mesh, scene, m);
		processMaterial(mesh, scene, mat);


		cout << "processMesh " << i + 1 << "/" << node->mNumMeshes << ", name " << mesh->mName.C_Str() << ", time " << t.tok() << endl;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		cout << "processNode " << i + 1 << "/" << node->mNumChildren << ", name " << node->mChildren[i]->mName.C_Str() << endl;
		processNode(node->mChildren[i], scene);
	}
}

void ObjLoader::processMesh(aiMesh * mesh, const aiScene * scene, Mesh* m)
{
	vector<RawVertexData> vertices;
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		RawVertexData vertex;
		glm::vec3 vector;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
		}

		// bitangent
		if (mesh->mBitangents)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	m->initByData(vertices, indices);
}


void ObjLoader::processMaterial(aiMesh * mesh, const aiScene *scene, Material* m)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	/* we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	 as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	 Same applies to other texture as the following list summarizes:
	 diffuse: texture_diffuseN
	 specular: texture_specularN
	 normal: texture_normalN*/

	 //1. diffuse maps
	loadMaterialTextures(material, aiTextureType_DIFFUSE, m);
	// 2. specular maps
	loadMaterialTextures(material, aiTextureType_SPECULAR, m);
	// 3. normal maps
	loadMaterialTextures(material, aiTextureType_HEIGHT, m);
	// 4. height maps
	loadMaterialTextures(material, aiTextureType_AMBIENT, m);

}

void ObjLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, Material* m)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		if (type == aiTextureType_DIFFUSE)
		{
			m->addTexture(str.C_Str(), "texture_diffuse" + std::to_string(i + 1));
		}
		else if (type == aiTextureType_SPECULAR)
		{
			m->addTexture(str.C_Str(), "texture_specular" + std::to_string(i + 1));
		}
		else if (type == aiTextureType_HEIGHT)
		{
			m->addTexture(str.C_Str(), "texture_normal" + std::to_string(i + 1));
		}
		else if (type == aiTextureType_AMBIENT)
		{
			m->addTexture(str.C_Str(), "texture_height" + std::to_string(i + 1));
		}
	}
}

NS_CC_END


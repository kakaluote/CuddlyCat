#include "Model.h"
#include "FileSystem.h"
#include "stb_image.h"
#include "Timer.h"
#include "ShaderCache.h"
NS_CC_BEGIN

Model::Model():
	_pos(0,0,0),
	_rot(0,0,0),
	_scale(1,1,1)
{
}


Model::~Model()
{
}

bool Model::init()
{
	_meshs.push_back(Mesh::initBox());

	for (int i = 0; i < _meshs.size(); i++) {
		_meshs[i].prepareGL();
	}

	return true;
}

bool Model::init2()
{
	_meshs.push_back(Mesh::initPlane());

	for (int i = 0; i < _meshs.size(); i++) {
		_meshs[i].prepareGL();
	}

	return true;
}

bool Model::initByGeometryType()
{
	return true;
}

bool Model::initByModelFile(const string& model_file) 
{
	_modelFileDir = model_file.substr(0, model_file.find_last_of('/'));
	loadModel(model_file);

	for (int i = 0; i < _meshs.size(); i++) {
		_meshs[i].prepareGL();
	}

	return true;
}

void Model::draw(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
	_shader->use();
	_shader->setMat4("projection", camera.getProjectionMatrix());
	_shader->setMat4("view", camera.getViewMatrix());
	_shader->setMat4("model", getTransformMat());

	_shader->setVec3("lightPos", lightPos);
	_shader->setVec3("viewPos", camera.getPos());
	_shader->setVec3("lightColor", lightColor);

	for (int i = 0; i < _meshs.size(); i++) {
		_meshs[i].draw(*_shader);
	}
}


void Model::setShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	_shader = ShaderCache::getInstance()->loadByFile(vertexPath, fragmentPath);
}

void Model::addTexture(const std::string& name, const std::string& path) 
{
	for (int i = 0; i < _meshs.size(); i++) {
		_meshs[i].addTexture(name, path);
	}
}

/////////////////////////////////////

void Model::loadModel(const string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Timer t;
		t.tik();
		
		processMesh(mesh, scene);

		cout << "processMesh " << i+1 << "/" << node->mNumMeshes << ", name " << mesh->mName.C_Str() <<", time "<<t.tok()<< endl;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		cout << "processNode " << i+1 << "/" << node->mNumChildren << ", name " << node->mChildren[i]->mName.C_Str() << endl;
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	vector<TextureInfo> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;
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
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;
		vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<TextureInfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<TextureInfo> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<TextureInfo> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	Mesh m(vertices, indices, textures);
	_meshs.push_back(m);
}

vector<TextureInfo>  Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<TextureInfo> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		TextureInfo texture;
		texture.name = typeName;
		texture.path = _modelFileDir + "/" + str.C_Str();
		textures.push_back(texture);
		cout << "loadMaterialTextures " << texture.name<<", path " <<texture.path << endl;
	}
	return textures;
}


NS_CC_END




#pragma once
#include "define.h"
#include <iostream>
#include "glad/gl.h"
#include "CuddlyCat/glm/glm.hpp"
#include <vector>
#include <string>
#include "Shader.h"
NS_CC_BEGIN

struct VertexData {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct TextureInfo {
	unsigned int id;
	std::string name;
	std::string path;
};

class Mesh
{
public:
	Mesh(const std::vector<VertexData>& vertices, 
		const std::vector<unsigned int>& indices, 
		const std::vector<TextureInfo>& textures);
	Mesh();
	~Mesh();

	void draw(Shader &shader);
	void prepareGL();

	static Mesh initPlane();
	static Mesh initBox();

	void addTexture(const std::string& name, const std::string& path);
	
private:
	std::vector<VertexData> _vertices;
	std::vector<unsigned int> _indices;

	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;

	std::vector<TextureInfo> _textures;
};

NS_CC_END
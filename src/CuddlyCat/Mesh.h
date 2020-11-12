#pragma once
#include "define.h"
#include <iostream>
#include "glad/gl.h"
#include "CuddlyCat/glm/glm.hpp"
#include <vector>
#include <string>
#include "Shader.h"
#include "ObjLoader.h"
NS_CC_BEGIN

struct RawVertexData;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void initByData(const std::vector<RawVertexData>& vertices,
		const std::vector<unsigned int>& indices);

	void initPlane();
	void initBox();

	void render();
	
private:
	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;

	unsigned int _vertexNum;
	unsigned int _indexNum;
};

NS_CC_END
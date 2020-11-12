#include "Mesh.h"

#include <fstream>
#include <sstream>
#include "TextureCache.h"

using namespace std;

NS_CC_BEGIN


Mesh::Mesh():
	_VAO(0),
	_VBO(0),
	_EBO(0),
	_vertexNum(0),
	_indexNum(0)
{
}

Mesh::~Mesh()
{
	if (_VAO != 0) {
		GL_CHECK(glDeleteVertexArrays(1, &_VAO));
	}
	if (_VBO != 0) {
		GL_CHECK(glDeleteBuffers(1, &_VBO));
	}
	if (_EBO != 0) {
		GL_CHECK(glDeleteBuffers(1, &_EBO));
	}
}

void Mesh::initByData(const std::vector<RawVertexData>& vertices, const std::vector<unsigned int>& indices)
{
	std::cout << "loadVertex " << vertices.size() << " " << indices.size() << std::endl;
	_vertexNum = vertices.size();
	_indexNum = indices.size();

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(RawVertexData), &vertices[0], GL_STATIC_DRAW);

	if (!indices.empty()) {
		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	// vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertexData), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertexData), (void*)offsetof(RawVertexData, normal));
	glEnableVertexAttribArray(1);

	// vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RawVertexData), (void*)offsetof(RawVertexData, texCoords));
	glEnableVertexAttribArray(2);

	// vertex tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertexData), (void*)offsetof(RawVertexData, tangent));
	glEnableVertexAttribArray(3);

	// vertex bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertexData), (void*)offsetof(RawVertexData, bitangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

void Mesh::initPlane()
{
	float pts[][3] = {
		{ 0.5f,  0.0f,  0.5f},  // top right
		{ 0.5f,  0.0f, -0.5f},  // bottom right
		{-0.5f,  0.0f, -0.5f},  // bottom left
		{-0.5f,  0.0f,  0.5f}   // top left 
	};

	std::vector<RawVertexData> vertices;
	for (int i = 0; i < 4; i++) {
		RawVertexData v;
		v.position.x = pts[i][0];
		v.position.y = pts[i][1];
		v.position.z = pts[i][2];
		v.normal.x = 0.0f;
		v.normal.y = 1.0f;
		v.normal.z = 0.0f;
		vertices.push_back(v);
	}
	std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };
	initByData(vertices, indices);
}

void Mesh::initBox()
{
	float pts[][8] = {
		// positions          // normals           // texture coords
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
		{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f},
		{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f},

		{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f},

		{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},

		{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{ 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f},

		{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},

		{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
		{ 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f}
	};


	std::vector<unsigned int> indices;

	std::vector<RawVertexData> vertices;
	for (int i = 0; i < 36; i++) {
		RawVertexData v;
		v.position.x = pts[i][0];
		v.position.y = pts[i][1];
		v.position.z = pts[i][2];

		v.normal.x = pts[i][3];
		v.normal.y = pts[i][4];
		v.normal.z = pts[i][5];

		v.texCoords.x = pts[i][6];
		v.texCoords.y = pts[i][7];

		vertices.push_back(v);
	}

	initByData(vertices, indices);
}

void Mesh::render()
{
	glBindVertexArray(_VAO);
	if (_indexNum > 0) {
		glDrawElements(GL_TRIANGLES, _indexNum, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, _vertexNum);
	}
	glBindVertexArray(0);
}


NS_CC_END




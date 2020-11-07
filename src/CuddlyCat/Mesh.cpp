#include "Mesh.h"

#include <fstream>
#include <sstream>
#include "TextureCache.h"

using namespace std;

NS_CC_BEGIN


Mesh::Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices,
	const std::vector<TextureInfo>& textures) :
	_vertices(vertices),
	_indices(indices),
	_textures(textures),
	_VAO(0),
	_VBO(0),
	_EBO(0)
{

}

Mesh::Mesh()
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

void Mesh::draw(Shader &shader)
{
	shader.use();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int j = 0; j < _textures.size(); j++)
	{
		string number;
		string name = _textures[j].name;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glActiveTexture(GL_TEXTURE0 + j);
		shader.setInt((name + number).c_str(), j);
		glBindTexture(GL_TEXTURE_2D, _textures[j].id);
	}

	glBindVertexArray(_VAO);

	if (!_indices.empty()) {
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	}
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::prepareGL()
{
	std::cout << "loadVertex " << _vertices.size() << " " << _indices.size() << std::endl;

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData), &_vertices[0], GL_STATIC_DRAW);

	if (!_indices.empty()) {
		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
	}

	// vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glEnableVertexAttribArray(1);

	// vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoords));
	glEnableVertexAttribArray(2);

	// vertex tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
	glEnableVertexAttribArray(3);

	// vertex bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, bitangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	for (int i = 0; i < _textures.size(); i++) {
		_textures[i].id = TextureCache::getInstance()->loadByFile(_textures[i].path);
	}
}

void Mesh::addTexture(const std::string& name, const std::string& path)
{
	TextureInfo info;
	info.name = name;
	info.path = path;
	info.id = TextureCache::getInstance()->loadByFile(path);
	_textures.push_back(info);
}

Mesh Mesh::initPlane()
{
	float pts[][3] = {
		{ 0.5f,  0.0f,  0.5f},  // top right
		{ 0.5f,  0.0f, -0.5f},  // bottom right
		{-0.5f,  0.0f, -0.5f},  // bottom left
		{-0.5f,  0.0f,  0.5f}   // top left 
	};

	std::vector<VertexData> vertices;
	for (int i = 0; i < 4; i++) {
		VertexData v;
		v.position.x = pts[i][0];
		v.position.y = pts[i][1];
		v.position.z = pts[i][2];
		v.normal.x = 0.0f;
		v.normal.y = 1.0f;
		v.normal.z = 0.0f;
		vertices.push_back(v);
	}
	std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };
	return Mesh(vertices, indices, std::vector<TextureInfo>());
}

Mesh Mesh::initBox()
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

	std::vector<VertexData> vertices;
	for (int i = 0; i < 36; i++) {
		VertexData v;
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


	return Mesh(vertices, indices, std::vector<TextureInfo>());
}

NS_CC_END




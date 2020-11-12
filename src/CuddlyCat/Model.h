#pragma once

#include "define.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "mesh.h"
#include "Material.h"
#include "Camera.h"

using namespace std;

NS_CC_BEGIN


class Material;
class Mesh;

class Model 
{
public:
	Model();
	~Model();

	void setMesh(Mesh* m) {
		_mesh = m;
	}
	void setMaterial(Material* m) {
		_material = m;
	}

	void render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor);

	const glm::vec3& pos() {
		return _pos;
	}
	const glm::vec3& rot() {
		return _rot;
	}
	const glm::vec3& scale() {
		return _scale;
	}

	void setPos(float x, float y, float z) {
		_pos.x = x;
		_pos.y = y;
		_pos.z = z;
	}
	void setRot(float x, float y, float z) {
		_rot.x = x;
		_rot.y = y;
		_rot.z = z;
	}
	void setScale(float x, float y, float z) {
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}
	void setScale(float s) {
		_scale.x = s;
		_scale.y = s;
		_scale.z = s;
	}

private:
	glm::mat4 getTransformMat() 
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, _pos);
		mat = glm::scale(mat, _scale);

		mat = glm::rotate(mat, glm::radians(_rot.x), glm::vec3(1, 0, 0));
		mat = glm::rotate(mat, glm::radians(_rot.y), glm::vec3(0, 1, 0));
		mat = glm::rotate(mat, glm::radians(_rot.z), glm::vec3(0, 0, 1));

		return mat;
	}


private:
	Mesh* _mesh;
	Material* _material;

	glm::vec3 _pos;
	glm::vec3 _rot;
	glm::vec3 _scale;
};





NS_CC_END
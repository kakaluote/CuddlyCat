#pragma once

#include "define.h"
#include "core/Object.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include <vector>
#include "Model.h"
using namespace std;

NS_CC_BEGIN

class Model;

class Node: public Object
{
public:
	Node();
	~Node();

	void addModel(Model* m)
	{
		_models.push_back(m);
	}

	void render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor);

private:
	std::vector<Model*> _models;
};





NS_CC_END
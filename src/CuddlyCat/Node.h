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

	void setModel(Model* m)
	{
		_model = m;
	}

	void render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor);

	void addChild(Node* node)
	{
		_children.push_back(node);
	}

private:
	Model* _model;
	std::vector<Node*> _children;
};





NS_CC_END
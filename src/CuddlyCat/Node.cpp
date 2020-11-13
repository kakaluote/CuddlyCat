#include "Node.h"

NS_CC_BEGIN

Node::Node():_model(nullptr)
{
}


Node::~Node()
{
	CC_SAFE_DELETE(_model);
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		delete *it;
	}
	_children.clear();
}

void Node::render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
	if (_model)
	{
		_model->render(camera, lightPos, lightColor);
	}
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->render(camera, lightPos, lightColor);
	}
}


NS_CC_END



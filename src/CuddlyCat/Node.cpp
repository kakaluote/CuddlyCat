#include "Node.h"

NS_CC_BEGIN

Node::Node()
{
}


Node::~Node()
{
	for (int i = 0; i < _models.size(); i++) {
		delete _models[i];
	}
}

void Node::render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
	for (int i = 0; i < _models.size(); i++) {
		_models[i]->render(camera, lightPos, lightColor);
	}
}


NS_CC_END



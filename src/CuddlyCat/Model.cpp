#include "Model.h"
#include "FileSystem.h"


NS_CC_BEGIN

Model::Model():
	_pos(0,0,0),
	_rot(0,0,0),
	_scale(1,1,1),
	_mesh(nullptr),
	_material(nullptr)
{
}


Model::~Model()
{
	CC_SAFE_DELETE(_mesh);
	CC_SAFE_DELETE(_material);
}

void Model::render(const Camera& camera, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
	_material->use(getTransformMat(), camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPos(), lightPos, lightColor);
	_mesh->render();
	_material->end();
}



NS_CC_END




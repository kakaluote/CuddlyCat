#include "Demo4.h"
#include "glad/gl.h"
#include <iostream>
#include "CuddlyCat/stb_image.h"
#include "CuddlyCat/FileSystem.h"

#include "CuddlyCat/glm/glm.hpp"
#include "CuddlyCat/glm/gtc/matrix_transform.hpp"
#include "CuddlyCat/glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "CuddlyCat/InputManager.h"
#include <functional>
#include "CuddlyCat/Director.h"
#include "ObjLoader.h"


Demo4::Demo4()
{
	_windowWidth = 800;
	_windowHeight = 600;
	_lastX = _windowWidth / 2.0f;
	_lastY = _windowHeight / 2.0f;

	_keyWState = GLFW_RELEASE;
	_keyAState = GLFW_RELEASE;
	_keySState = GLFW_RELEASE;
	_keyDState = GLFW_RELEASE;
	_keyQState = GLFW_RELEASE;
	_keyEState = GLFW_RELEASE;
}

Demo4::~Demo4()
{
	for (int i = 0; i < _nodes.size(); i++) {
		delete _nodes[i];
	}
}

bool Demo4::init()
{
	BaseDemo::init();
	return true;
}

void Demo4::prepare()
{
	Node* node;

	node = ObjLoader::getInstance()->loadModel(FileSystem::getInstance()->getPath("Resources/objects/rock/rock.obj"));
	if (node)
	{
		_nodes.push_back(node);
	}


	//Model* model;

	//// backpack  rock  planet cyborg  nanosuit
	//model = new Model;
	//_models.push_back(model);


	//model->initByModelFile(FileSystem::getInstance()->getPath("Resources/objects/rock/rock.obj"));
	//model->setShader("../shader/myshader/simple4.vs", "../shader/myshader/simple4.fs");
	//model->setPos(0, -2, 0);
	//model->setScale(2);
	//model->setRot(0, 90, 0);

	

	//model = new Model;
	//_models.push_back(model);

	//model->init2();
	//model->setShader("Resources/shader/myshader/simple3.vs", "Resources/shader/myshader/simple3.fs");
	////model->setPos(0, -2, 0);
	//model->setScale(10);


	//model = new Model;
	//_models.push_back(model);

	//model->init();
	//model->setShader("Resources/shader/myshader/simple2.vs", "Resources/shader/myshader/simple2.fs");
	//model->addTexture("texture_diffuse1", "Resources/textures/container2.png");
	//model->addTexture("texture_specular1", "Resources/textures/container2_specular.png");
	//model->setPos(-2, 0.5, 0);
	////model->setScale(10);


	//model = new Model;
	//_models.push_back(model);

	//model->init();
	//model->setShader("Resources/shader/myshader/simple3.vs", "Resources/shader/myshader/simple3.fs");
	//model->setPos(2, 1, 0);


	_camera.setPos(0, 3, 8);
	_camera.setPitch(-10.0f);
	//_camera.setYaw(-100.0f);

	_lightPos = glm::vec3(-2, 1.5f, 1.0f);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Demo4::render()
{
	handlerInput();

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < _nodes.size(); i++) {
		_nodes[i]->render(_camera, _lightPos, glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void Demo4::cleanup()
{
}

void Demo4::onMouseCallback(int button, int action, int modify)
{

}

void Demo4::onMouseMoveCallBack(double x, double y)
{
	if (_firstMouse)
	{
		_lastX = x;
		_lastY = y;
		_firstMouse = false;
	}

	float xoffset = x - _lastX;
	float yoffset = _lastY - y; // reversed since y-coordinates go from bottom to top
	_lastX = x;
	_lastY = y;

	_camera.changeDir(xoffset, yoffset);
}

void Demo4::onMouseScrollCallback(double x, double y)
{
	_camera.zoom(y);
}

void Demo4::onKeyCallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {

		case GLFW_KEY_W:
		{
			_keyWState = GLFW_PRESS;
		}
		break;
		case GLFW_KEY_S:
		{
			_keySState = GLFW_PRESS;
		}
		break;
		case GLFW_KEY_A:
		{
			_keyAState = GLFW_PRESS;
		}
		break;
		case GLFW_KEY_D:
		{
			_keyDState = GLFW_PRESS;
		}
		break;
		case GLFW_KEY_Q:
		{
			_keyQState = GLFW_PRESS;
		}
		break;
		case GLFW_KEY_E:
		{
			_keyEState = GLFW_PRESS;
		}
		break;

		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {

		case GLFW_KEY_W:
		{
			_keyWState = GLFW_RELEASE;
		}
		break;
		case GLFW_KEY_S:
		{
			_keySState = GLFW_RELEASE;
		}
		break;
		case GLFW_KEY_A:
		{
			_keyAState = GLFW_RELEASE;
		}
		break;
		case GLFW_KEY_D:
		{
			_keyDState = GLFW_RELEASE;
		}
		break;
		case GLFW_KEY_Q:
		{
			_keyQState = GLFW_RELEASE;
		}
		break;
		case GLFW_KEY_E:
		{
			_keyEState = GLFW_RELEASE;
		}
		break;

		}
	}
}

void Demo4::handlerInput()
{
	if(_keyWState == GLFW_PRESS){
		_camera.move(FORWARD, Director::getInstance()->getDeltaTime());
	}
	else if (_keySState == GLFW_PRESS) {
		_camera.move(BACKWARD, Director::getInstance()->getDeltaTime());
	}
	if (_keyAState == GLFW_PRESS) {
		_camera.move(LEFT, Director::getInstance()->getDeltaTime());
	}
	else if (_keyDState == GLFW_PRESS) {
		_camera.move(RIGHT, Director::getInstance()->getDeltaTime());
	}
	if (_keyQState == GLFW_PRESS) {
		_camera.move(UP, Director::getInstance()->getDeltaTime());
	}
	else if (_keyEState == GLFW_PRESS) {
		_camera.move(DOWN, Director::getInstance()->getDeltaTime());
	}
}

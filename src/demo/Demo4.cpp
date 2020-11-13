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

#include "yaml-cpp/yaml.h"
#include "spdlog/spdlog.h"
#include <nlohmann/json.hpp>
#include <iosfwd>
using json = nlohmann::json;

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

	YAML::Node config = YAML::LoadFile("Resources/config.yaml");
	std::cout << config << std::endl;

	spdlog::info("Welcome to spdlog!");
	spdlog::error("Some error message with arg: {}", 1);

	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	spdlog::info("Support for floats {:03.2f}", 1.23456);
	spdlog::info("Positional args are {1} {0}..", "too", "supported");
	spdlog::info("{:<30}", "left aligned");

	spdlog::set_level(spdlog::level::debug); // Set global log level to debug
	spdlog::debug("This message should be displayed..");

	// change log pattern
	spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

	// Compile time log levels
	// define SPDLOG_ACTIVE_LEVEL to desired level
	SPDLOG_TRACE("Some trace message with param {}", 42);
	SPDLOG_DEBUG("Some debug message");

	json j = {
	  {"pi", 3.141},
	  {"happy", true},
	  {"name", "Niels"},
	  {"nothing", nullptr},
	  {"answer", {
		{"everything", 42}
	  }},
	  {"list", {1, 0, 2}},
	  {"object", {
		{"currency", "USD"},
		{"value", 42.99}
	  }}
	};
	spdlog::debug(j.dump());

	std::ofstream f1("Resources/test.json");
	if (f1)
	{
		f1 << j.dump();
		f1.close();
	}

	/*std::ifstream f2("Resources/test.json");
	if (f2)
	{
		f2.read()
	}
	json j2;
	j2.parse()*/

	return true;
}

void Demo4::prepare()
{
	Node* node;

	node = ObjLoader::getInstance()->loadModel(FileSystem::getInstance()->getPath("Resources/objects/nanosuit/nanosuit.obj"), "Resources/objects/nanosuit/");
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

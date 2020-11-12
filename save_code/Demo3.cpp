#include "Demo3.h"
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


Demo3::Demo3()
{
	_windowWidth = 800;
	_windowHeight = 600;
	_lastX = _windowWidth / 2.0f;
	_lastY = _windowHeight / 2.0f;

	_keyWState = GLFW_RELEASE;
	_keyAState = GLFW_RELEASE;
	_keySState = GLFW_RELEASE;
	_keyDState = GLFW_RELEASE;
}

Demo3::~Demo3()
{

}

bool Demo3::init()
{
	BaseDemo::init();
	return true;
}

void Demo3::prepare()
{
	_lightShader.initByFile("Resources/shader/5.2.light_cube.vs", "Resources/shader/5.2.light_cube.fs");
	_cubeShader.initByFile("Resources/shader/5.2.light_casters.vs", "Resources/shader/5.2.light_casters.fs");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glGenVertexArrays(1, &_lightVAO);
	glBindVertexArray(_lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_diffuseMap.initByFile(FileSystem::getInstance()->getPath("Resources/textures/container2.png"));
	_specularMap.initByFile(FileSystem::getInstance()->getPath("Resources/textures/container2_specular.png"));

	_cubeShader.use();
	_cubeShader.setInt("material.diffuse", 0);
	_cubeShader.setInt("material.specular", 1);

	_camera.setPos(1, 2, 4);
	_camera.setPitch(-20.0f);
	_camera.setYaw(-100.0f);

	_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);


	glEnable(GL_DEPTH_TEST);
}

void Demo3::render()
{
	handlerInput();

	// render
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	
	glm::mat4 projection = _camera.getProjectionMatrix();
	glm::mat4 view = _camera.getViewMatrix();

	_cubeShader.use();
	_cubeShader.setMat4("projection", projection);
	_cubeShader.setMat4("view", view);

	glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePos);
	_cubeShader.setMat4("model", model);

	_cubeShader.setVec3("light.position", _lightPos);
	//_cubeShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	_cubeShader.setVec3("viewPos", _camera.getPos());

	// light properties
	_cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	_cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	_cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	_cubeShader.setFloat("light.constant", 1.0f);
	_cubeShader.setFloat("light.linear", 0.09f);
	_cubeShader.setFloat("light.quadratic", 0.032f);

	// material properties
	_cubeShader.setFloat("material.shininess", 32.0f);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseMap.id());
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specularMap.id());

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	_lightShader.use();
	
	_lightShader.setMat4("projection", projection);
	_lightShader.setMat4("view", view);

	model = glm::mat4(1.0f);
	model = glm::translate(model, _lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	_lightShader.setMat4("model", model);

	glBindVertexArray(_lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	
}

void Demo3::cleanup()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	//glDeleteBuffers(1, &_EBO);

	glDeleteVertexArrays(1, &_lightVAO);
}

void Demo3::onMouseCallback(int button, int action, int modify)
{

}

void Demo3::onMouseMoveCallBack(double x, double y)
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

void Demo3::onMouseScrollCallback(double x, double y)
{
	_camera.zoom(y);
}

void Demo3::onKeyCallback(int key, int scancode, int action, int mods)
{
	_keyWState = GLFW_RELEASE;
	_keyAState = GLFW_RELEASE;
	_keySState = GLFW_RELEASE;
	_keyDState = GLFW_RELEASE;

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
		{
			_keyWState = GLFW_PRESS;
			_camera.move(FORWARD, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_S:
		{
			_keySState = GLFW_PRESS;
			_camera.move(BACKWARD, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_A:
		{
			_keyAState = GLFW_PRESS;
			_camera.move(LEFT, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_D:
		{
			_keyDState = GLFW_PRESS;
			_camera.move(RIGHT, Director::getInstance()->getDeltaTime());
		}
		break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W:
		{
			_keyWState = GLFW_RELEASE;
			_camera.move(FORWARD, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_S:
		{
			_keySState = GLFW_RELEASE;
			_camera.move(BACKWARD, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_A:
		{
			_keyAState = GLFW_RELEASE;
			_camera.move(LEFT, Director::getInstance()->getDeltaTime());
		}
		break;
		case GLFW_KEY_D:
		{
			_keyDState = GLFW_RELEASE;
			_camera.move(RIGHT, Director::getInstance()->getDeltaTime());
		}
		break;
		}
	}
}

void Demo3::handlerInput()
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
}

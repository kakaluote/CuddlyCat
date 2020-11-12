#pragma once
#include "CuddlyCat/define.h"
#include "BaseDemo.h"
#include "CuddlyCat/Camera.h"
#include "Node.h"

USING_NS_CC

class Demo4 : public BaseDemo
{
public:
	Demo4();
	~Demo4();
	virtual bool init();

	void prepare() override;
	void render() override;
	void cleanup() override;

protected:
	void onMouseCallback(int button, int action, int modify) override;
	void onMouseMoveCallBack(double x, double y) override;
	void onMouseScrollCallback(double x, double y) override;
	void onKeyCallback(int key, int scancode, int action, int mods) override;

private:
	void handlerInput();

private:
	int _windowWidth;
	int _windowHeight;

	float _lastX;
	float _lastY;
	bool _firstMouse;
	int _keyWState;
	int _keyAState;
	int _keySState;
	int _keyDState;
	int _keyQState;
	int _keyEState;

	Camera _camera;
	glm::vec3 _lightPos;
	std::vector<Node*> _nodes;
};


#pragma once
#include "CuddlyCat/define.h"
#include "BaseDemo.h"
#include "CuddlyCat/Shader.h"
#include "CuddlyCat/Camera.h"

USING_NS_CC

class Demo2 : public BaseDemo
{
public:
	Demo2();
	~Demo2();
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
	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;
	Shader _cubeShader;
	unsigned int _texture1;
	unsigned int _texture2;

	int _windowWidth;
	int _windowHeight;

	Camera _camera;

	float _lastX;
	float _lastY;
	bool _firstMouse;
};


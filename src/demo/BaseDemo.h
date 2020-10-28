#pragma once

class BaseDemo
{
public:
	BaseDemo();
	virtual ~BaseDemo();
	virtual bool init();

	virtual void prepare() = 0;
	virtual void render() = 0;
	virtual void cleanup() = 0;

protected:
	virtual void onMouseCallback(int button, int action, int modify);
	virtual void onMouseMoveCallBack(double x, double y);
	virtual void onMouseScrollCallback(double x, double y);
	virtual void onKeyCallback(int key, int scancode, int action, int mods);
};


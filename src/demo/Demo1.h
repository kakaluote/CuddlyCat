#pragma once
#include "BaseDemo.h"
class Demo1 : public BaseDemo
{
public:
	Demo1();
	~Demo1();
	virtual bool init();

	virtual void prepare();
	virtual void render();
	virtual void cleanup();

private:
	int _shaderProgram;
	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;
};


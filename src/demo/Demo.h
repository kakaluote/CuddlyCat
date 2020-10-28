#pragma once
#include "BaseDemo.h"
class Demo : public BaseDemo
{
public:
	Demo();
	~Demo();
	virtual bool init();

	virtual void prepare();
	virtual void render();
	virtual void cleanup();

private:
};


#pragma once

#include "BaseDemo.h"


class DemoCtrl
{
public:
	static DemoCtrl* getInstance();
	static void destroyInstance();

	DemoCtrl();
	~DemoCtrl();
	bool init();

	BaseDemo* getDemo(int idx);

private:
	void deleteDemoHandler();

private:
	int _demoIdx;
	BaseDemo* _demoHandler;
};


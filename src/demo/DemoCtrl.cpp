#include "DemoCtrl.h"
#include "Demo1.h"
#include "Demo2.h"
#include "Demo4.h"

static DemoCtrl *s_SharedDemoCtrl = nullptr;

DemoCtrl* DemoCtrl::getInstance()
{
	if (!s_SharedDemoCtrl)
	{
		s_SharedDemoCtrl = new DemoCtrl;
		s_SharedDemoCtrl->init();
	}

	return s_SharedDemoCtrl;
}

void DemoCtrl::destroyInstance()
{
	if (s_SharedDemoCtrl != nullptr)
	{
		delete s_SharedDemoCtrl;
		s_SharedDemoCtrl = nullptr;
	}
}

DemoCtrl::DemoCtrl():_demoHandler(nullptr), _demoIdx(0)
{

}

DemoCtrl::~DemoCtrl()
{
	deleteDemoHandler();
}

bool DemoCtrl::init()
{
	return true;
}

void DemoCtrl::deleteDemoHandler()
{
	if (_demoHandler != nullptr)
	{
		delete _demoHandler;
		_demoHandler = nullptr;
	}
}


BaseDemo* DemoCtrl::getDemo(int idx)
{
	if (_demoIdx != idx) {
		deleteDemoHandler();
	}

	_demoIdx = idx;
	if (idx == 1) {
		_demoHandler = new Demo1();
	}
	else if (idx == 2) {
		_demoHandler = new Demo2();
	}
	else if (idx == 4) {
		_demoHandler = new Demo4();
	}

	if (_demoHandler != nullptr) {
		_demoHandler->init();
	}
	
	return _demoHandler;
}



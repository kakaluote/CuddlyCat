#include "BaseDemo.h"
#include "CuddlyCat/InputManager.h"

USING_NS_CC

BaseDemo::BaseDemo()
{

}

BaseDemo::~BaseDemo()
{

}

bool BaseDemo::init()
{
	InputManager::getInstance()->registerMouseListener(CC_CALLBACK_3(BaseDemo::onMouseCallback, this));
	InputManager::getInstance()->registerMouseMoveListener(CC_CALLBACK_2(BaseDemo::onMouseMoveCallBack, this));
	InputManager::getInstance()->registerMouseScrollListener(CC_CALLBACK_2(BaseDemo::onMouseScrollCallback, this));
	InputManager::getInstance()->registerKeyBoardListener(CC_CALLBACK_4(BaseDemo::onKeyCallback, this));

	return true;
}

void BaseDemo::onMouseCallback(int button, int action, int modify)
{

}

void BaseDemo::onMouseMoveCallBack(double x, double y)
{

}

void BaseDemo::onMouseScrollCallback(double x, double y)
{

}

void BaseDemo::onKeyCallback(int key, int scancode, int action, int mods)
{

}


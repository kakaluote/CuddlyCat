#include "InputManager.h"

NS_CC_BEGIN

static InputManager *s_SharedInputManager = nullptr;

InputManager* InputManager::getInstance()
{
	if (!s_SharedInputManager)
	{
		s_SharedInputManager = new InputManager;
		s_SharedInputManager->init();
	}

	return s_SharedInputManager;
}
void InputManager::destroyInstance()
{
	if (s_SharedInputManager != nullptr)
	{
		delete s_SharedInputManager;
		s_SharedInputManager = nullptr;
	}
}

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool InputManager::init()
{
	return true;
}

void InputManager::onMouseCallBack(int button, int action, int modify)
{
	for (int i = 0; i < _mouseListeners.size(); i++) {
		_mouseListeners[i](button, action, modify);
	}
}

void InputManager::onMouseMoveCallBack(double x, double y)
{
	for (int i = 0; i < _mouseMoveListeners.size(); i++) {
		_mouseMoveListeners[i](x, y);
	}
}

void InputManager::onMouseScrollCallback(double x, double y)
{
	for (int i = 0; i < _mouseScrollListeners.size(); i++) {
		_mouseScrollListeners[i](x, y);
	}
}

void InputManager::onKeyCallback(int key, int scancode, int action, int mods)
{
	for (int i = 0; i < _keyBoardListeners.size(); i++) {
		_keyBoardListeners[i](key, scancode, action, mods);
	}
}

void InputManager::onCharCallback(unsigned int character)
{
}


NS_CC_END
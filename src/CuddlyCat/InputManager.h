#pragma once
#include "define.h"
#include <functional>
#include <vector>

NS_CC_BEGIN


class InputManager
{
public:
	static InputManager* getInstance();
	static void destroyInstance();

	InputManager();
	~InputManager();
	bool init();

	void onMouseCallBack(int button, int action, int modify);
	void onMouseMoveCallBack(double x, double y);
	void onMouseScrollCallback(double x, double y);
	void onKeyCallback(int key, int scancode, int action, int mods);
	void onCharCallback(unsigned int character);

	void registerMouseListener(const MouseListener& listener) {
		_mouseListeners.push_back(listener);
	}
	void registerMouseMoveListener(const MouseMoveListener& listener) {
		_mouseMoveListeners.push_back(listener);
	}
	void registerMouseScrollListener(const MouseScrollListener& listener) {
		_mouseScrollListeners.push_back(listener);
	}
	void registerKeyBoardListener(const KeyBoardListener& listener) {
		_keyBoardListeners.push_back(listener);
	}

private:
	std::vector<MouseListener> _mouseListeners;
	std::vector<MouseMoveListener> _mouseMoveListeners;
	std::vector<MouseScrollListener> _mouseScrollListeners;
	std::vector<KeyBoardListener> _keyBoardListeners;
};

NS_CC_END
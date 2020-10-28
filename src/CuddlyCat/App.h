#pragma once
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <chrono>
#include <thread>
#include "define.h"
#include "demo/BaseDemo.h"
#include "InputManager.h"

NS_CC_BEGIN

class App
{
public:
	App();
	~App();

	bool init();

	void mainLoop();

	void render();
	void renderUI();

	void setDemoHandler(BaseDemo* h);

private:
	void demoPrepare();
	void demoRender();
	void demoCleanup();
private:
	GLFWwindow *_mainWindow;
	int _windowWidth;
	int _windowHeight;
	std::string _windowTitle;

	BaseDemo* _demoHandler;
};

class GLFWEventHandler
{
public:
	static void onGLFWError(int errorID, const char* errorDesc)
	{
		fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", errorID, errorDesc);
	}

	static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
	{
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modify);
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}
		InputManager::getInstance()->onMouseCallBack(button, action, modify);
	}

	static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
	{
		InputManager::getInstance()->onMouseMoveCallBack(x, y);
	}

	static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
	{
		ImGui_ImplGlfw_ScrollCallback(window, x, y);
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}
		InputManager::getInstance()->onMouseScrollCallback(x, y);
	}

	static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE) {
			// Quit
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		}
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
		if (ImGui::GetIO().WantCaptureKeyboard) {
			return;
		}
		InputManager::getInstance()->onKeyCallback(key, scancode, action, mods);
	}

	static void onGLFWCharCallback(GLFWwindow* window, unsigned int character)
	{
		ImGui_ImplGlfw_CharCallback(window, character);
		InputManager::getInstance()->onCharCallback(character);
	}

	static void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
	{
	}

	static void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
	{
	}

	static void onGLFWframebuffersize(GLFWwindow *window, int width, int height)
	{
	}

	static void onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified)
	{
	}

	static void onGLFWWindowFocusCallback(GLFWwindow* window, int focused)
	{
	}
};

NS_CC_END
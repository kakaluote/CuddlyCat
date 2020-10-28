#include "App.h"
#include "FileSystem.h"
#include "Director.h"
#include "TextureCache.h"
#include "ShaderCache.h"


NS_CC_BEGIN

App::App(): _demoHandler(nullptr)
{

}

App::~App()
{
	Director::destroyInstance();
	FileSystem::destroyInstance();
	TextureCache::destroyInstance();
	ShaderCache::destroyInstance();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
}

bool App::init()
{
	_windowWidth = 800;
	_windowHeight = 600;
	_windowTitle = "Demo";

	glfwSetErrorCallback(GLFWEventHandler::onGLFWError);

	if (glfwInit() == 0) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_mainWindow = glfwCreateWindow(_windowWidth, _windowHeight, _windowTitle.c_str(), NULL, NULL);
	if (_mainWindow == nullptr) {
		fprintf(stderr, "Failed to open GLFW g_mainWindow.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_mainWindow);

	// Load OpenGL functions using glad
	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSetMouseButtonCallback(_mainWindow, GLFWEventHandler::onGLFWMouseCallBack);
	glfwSetCursorPosCallback(_mainWindow, GLFWEventHandler::onGLFWMouseMoveCallBack);
	glfwSetScrollCallback(_mainWindow, GLFWEventHandler::onGLFWMouseScrollCallback);
	glfwSetCharCallback(_mainWindow, GLFWEventHandler::onGLFWCharCallback);
	glfwSetKeyCallback(_mainWindow, GLFWEventHandler::onGLFWKeyCallback);
	glfwSetWindowPosCallback(_mainWindow, GLFWEventHandler::onGLFWWindowPosCallback);
	glfwSetFramebufferSizeCallback(_mainWindow, GLFWEventHandler::onGLFWframebuffersize);
	glfwSetWindowSizeCallback(_mainWindow, GLFWEventHandler::onGLFWWindowSizeFunCallback);
	glfwSetWindowIconifyCallback(_mainWindow, GLFWEventHandler::onGLFWWindowIconifyCallback);
	glfwSetWindowFocusCallback(_mainWindow, GLFWEventHandler::onGLFWWindowFocusCallback);

	glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	bool success;
	success = ImGui_ImplGlfw_InitForOpenGL(_mainWindow, false);
	if (success == false) {
		printf("ImGui_ImplGlfw_InitForOpenGL failed\n");
		assert(false);
	}

	success = ImGui_ImplOpenGL3_Init();
	if (success == false) {
		printf("ImGui_ImplOpenGL3_Init failed\n");
		assert(false);
	}

	// Search for font file
	const char *fontPath = "data/droid_sans.ttf";
	FILE *file = fopen(fontPath, "rb");
	if (file) {
		fclose(file);
		ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 13.0f);
	}
	//ImGui::GetIO().FontGlobalScale = 1.5;


	return true;
}

void App::mainLoop()
{
	demoPrepare();

	std::chrono::duration<double> sleepAdjust(0.0);
	std::chrono::duration<double> deltaTime;
	std::chrono::duration<double> target(1.0 / 60.0);

	while (!glfwWindowShouldClose(_mainWindow)) {
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		glfwGetWindowSize(_mainWindow, &_windowWidth, &_windowHeight);
		int bufferWidth, bufferHeight;
		glfwGetFramebufferSize(_mainWindow, &bufferWidth, &bufferHeight);
		glViewport(0, 0, bufferWidth, bufferHeight);

		render();
		renderUI();

		glfwSwapBuffers(_mainWindow);
		glfwPollEvents();

		// Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
		// using mm_pause or equivalent for the last millisecond.
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeUsed = t2 - t1;
		std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
		if (sleepTime > std::chrono::duration<double>(0)) {
			std::this_thread::sleep_for(sleepTime);
		}

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		deltaTime = t3 - t1;
		Director::getInstance()->setDeltaTime(1000.0 * deltaTime.count());

		// Compute the sleep adjustment using a low pass filter
		sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - deltaTime);
	}

	demoCleanup();
}

void App::render()
{
	demoRender();
}

void App::renderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(10, ImGui::GetIO().DisplaySize.y - 10), ImGuiCond_Always, ImVec2(0, 1));
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::Begin("Status", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav);

	ImGui::SetCursorPos(ImVec2(float(5), float(5)));
	char buffer[128];
	sprintf(buffer, "%.1f ms   ", Director::getInstance()->getDeltaTime());
	ImGui::TextColored(ImColor(230, 153, 153, 255), buffer);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::setDemoHandler(BaseDemo* h)
{
	_demoHandler = h;
}

void App::demoPrepare()
{
	if (_demoHandler == nullptr) {
		return;
	}
	_demoHandler->prepare();
}

void App::demoRender()
{
	if (_demoHandler == nullptr) {
		return;
	}
	_demoHandler->render();
}

void App::demoCleanup()
{
	if (_demoHandler == nullptr) {
		return;
	}
	_demoHandler->cleanup();
}

NS_CC_END




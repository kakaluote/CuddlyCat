
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "draw.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "RVO.h"
#include "PathFindSim.h"
#include <iostream>

#define M_PI 3.1415926

#if defined(_WIN32)
#include <crtdbg.h>
#endif
using namespace std;


GLFWwindow *g_mainWindow = nullptr;
DebugDraw g_debugDraw;
int g_curScenario = 3;
float goal_small_dist = 0.1f;

bool g_left_mouse_btn_down = false;
double g_last_cursor_x = 0;
double g_last_cursor_y = 0;
double g_cur_cursor_x = 0;
double g_cur_cursor_y = 0;
double g_cursor_x_delta = 0;
double g_cursor_y_delta = 0;
b2Vec2 s_clickPointWS;
b2Vec2 s_rightClickPointWS;

float g_camera_extent = 15;
b2Vec2 g_camera_center(17,13);

//PathFindSim g_pfs(32,26,1);
PathFindSim g_pfs(5, 5, 1);


void glfwErrorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static void ResizeWindowCallback(GLFWwindow *, int width, int height) {
}

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Quit
                glfwSetWindowShouldClose(g_mainWindow, GL_TRUE);
                break;

            case GLFW_KEY_LEFT:
                // Pan left
                if (mods == GLFW_MOD_CONTROL) {
                } else {
                }
                break;

            case GLFW_KEY_RIGHT:
                // Pan right
                break;

            case GLFW_KEY_DOWN:
                // Pan down
                break;

            case GLFW_KEY_UP:
                // Pan up
                break;

            case GLFW_KEY_HOME:
                // Reset view
                break;

            case GLFW_KEY_Z:
                // Zoom out
                break;

            case GLFW_KEY_X:
                // Zoom in
                break;

            case GLFW_KEY_R:
                // Reset test
                break;

            case GLFW_KEY_SPACE:
                // Launch a bomb.
                break;

            case GLFW_KEY_O:
                break;

            case GLFW_KEY_P:
                break;

            case GLFW_KEY_LEFT_BRACKET:
                break;

            case GLFW_KEY_RIGHT_BRACKET:
                break;

            case GLFW_KEY_TAB:
                break;
        }
    } else if (action == GLFW_RELEASE) {
    }
    // else GLFW_REPEAT
}

static void CharCallback(GLFWwindow *window, unsigned int c) {
    ImGui_ImplGlfw_CharCallback(window, c);
}

static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (ImGui::GetIO().WantCaptureMouse) {
		return;
	}

    double xd, yd;
    glfwGetCursorPos(g_mainWindow, &xd, &yd);
    b2Vec2 ps((float)xd, (float)yd);
	b2Vec2 pt_ws = g_camera.ConvertScreenToWorld(ps);

    // Use the mouse to move things around.
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			if (mods == GLFW_MOD_CONTROL) {
				s_clickPointWS = pt_ws;
				g_left_mouse_btn_down = true;
			}
			else {
				if (g_pfs.is_block(pt_ws.x, pt_ws.y)) {
					g_pfs.clear_block(pt_ws.x, pt_ws.y);
				}
				else {
					g_pfs.set_block(pt_ws.x, pt_ws.y);
				}
			}
        }

        if (action == GLFW_RELEASE) {
            g_left_mouse_btn_down = false;
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
			s_rightClickPointWS = pt_ws;
			g_pfs.move_to(pt_ws);
        }

        if (action == GLFW_RELEASE) {
        }
    }
}

static void MouseMotionCallback(GLFWwindow *, double xd, double yd) {
    b2Vec2 ps((float)xd, (float)yd);
    b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

    if(g_left_mouse_btn_down){
        b2Vec2 diff = pw - s_clickPointWS;
        g_camera.m_center.x -= diff.x;
        g_camera.m_center.y -= diff.y;
        s_clickPointWS = g_camera.ConvertScreenToWorld(ps);
    }
}

static void ScrollCallback(GLFWwindow *window, double dx, double dy) {
    ImGui_ImplGlfw_ScrollCallback(window, dx, dy);
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    if (dy > 0) {
        g_camera.m_zoom /= 1.1f;
    } else {
        g_camera.m_zoom *= 1.1f;
    }
}


/////////////////////////////////////

void setupScenario1(RVO::RVOSimulator *sim) {
#if RVO_SEED_RANDOM_NUMBER_GENERATOR
    std::srand(static_cast<unsigned int>(std::time(NULL)));
#endif

    size_t agentNo = 0;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            agentNo = sim->addAgent(RVO::Vector2(55.0f + i * 10.0f, 55.0f + j * 10.0f));
            sim->setAgentGoal(agentNo, -75.0f, -75.0f);

            agentNo = sim->addAgent(RVO::Vector2(-55.0f - i * 10.0f, 55.0f + j * 10.0f));
            sim->setAgentGoal(agentNo, 75.0f, -75.0f);

            agentNo = sim->addAgent(RVO::Vector2(55.0f + i * 10.0f, -55.0f - j * 10.0f));
            sim->setAgentGoal(agentNo, -75.0f, 75.0f);

            agentNo = sim->addAgent(RVO::Vector2(-55.0f - i * 10.0f, -55.0f - j * 10.0f));
            sim->setAgentGoal(agentNo, 75.0f, 75.0f);
        }
    }

    /*
     * Add (polygonal) obstacles, specifying their vertices in counterclockwise
     * order.
     */
    std::vector<std::vector<RVO::Vector2> > obstacles;
    std::vector<RVO::Vector2> obstacle1, obstacle2, obstacle3, obstacle4;

    obstacle1.push_back(RVO::Vector2(-10.0f, 40.0f));
    obstacle1.push_back(RVO::Vector2(-40.0f, 40.0f));
    obstacle1.push_back(RVO::Vector2(-40.0f, 10.0f));
    obstacle1.push_back(RVO::Vector2(-10.0f, 10.0f));

    obstacle2.push_back(RVO::Vector2(10.0f, 40.0f));
    obstacle2.push_back(RVO::Vector2(10.0f, 10.0f));
    obstacle2.push_back(RVO::Vector2(40.0f, 10.0f));
    obstacle2.push_back(RVO::Vector2(40.0f, 40.0f));

    obstacle3.push_back(RVO::Vector2(10.0f, -40.0f));
    obstacle3.push_back(RVO::Vector2(40.0f, -40.0f));
    obstacle3.push_back(RVO::Vector2(40.0f, -10.0f));
    obstacle3.push_back(RVO::Vector2(10.0f, -10.0f));

    obstacle4.push_back(RVO::Vector2(-10.0f, -40.0f));
    obstacle4.push_back(RVO::Vector2(-10.0f, -10.0f));
    obstacle4.push_back(RVO::Vector2(-40.0f, -10.0f));
    obstacle4.push_back(RVO::Vector2(-40.0f, -40.0f));

    sim->addObstacle(obstacle1);
    sim->addObstacle(obstacle2);
    sim->addObstacle(obstacle3);
    sim->addObstacle(obstacle4);

    /* Process the obstacles so that they are accounted for in the simulation. */
    sim->processObstacles();
}

void setupScenario2(RVO::RVOSimulator *sim) {
    size_t agentNo = 0;
    agentNo = sim->addAgent(RVO::Vector2(-50.0f, 0));
    sim->setAgentGoal(agentNo, 50, 0);
    agentNo = sim->addAgent(RVO::Vector2(50.0f, 0));
    sim->setAgentGoal(agentNo, -50, 0);
    agentNo = sim->addAgent(RVO::Vector2(50.0f, 10), 15, 10, 15,
                            15, 6, 2);
    sim->setAgentGoal(agentNo, -50, -10);
}

void setupScenario3(RVO::RVOSimulator *sim) {
    size_t agentNo = 0;
    for (size_t i = 0; i < 250; ++i) {
        agentNo = sim->addAgent(200.0f *
                                RVO::Vector2(std::cos(i * 2.0f * M_PI / 250.0f),
                                             std::sin(i * 2.0f * M_PI / 250.0f)));
        sim->setAgentGoal(agentNo, -sim->getAgentPosition(i));
    }
}

void setupScenario4() {

}

const char *items[] = {"Scenario1", "Scenario2", "Scenario3", "Scenario4"};

void setupScenario(RVO::RVOSimulator *sim) {
    if (g_curScenario == 0) {
        setupScenario1(sim);
    } else if (g_curScenario == 1) {
        setupScenario2(sim);
    } else if (g_curScenario == 2) {
        setupScenario3(sim);
    } else if (g_curScenario == 3) {
        setupScenario4();
    }
}

void setPreferredVelocities(RVO::RVOSimulator *sim) {
    /*
     * Set the preferred velocity to be a vector of unit magnitude (speed) in the
     * direction of the goal.
     */
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i) {
        RVO::Vector2 goalVector = sim->getAgentGoal(i) - sim->getAgentPosition(i);

        if (RVO::absSq(goalVector) > 1.0f) {
            goalVector = RVO::normalize(goalVector);
        }

        /*
         * Perturb a little to avoid deadlocks due to perfect symmetry.
         */
        float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
        float dist = std::rand() * 0.0001f / RAND_MAX;

        sim->setAgentPrefVelocity(i, goalVector +
                                     dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
    }
}

bool reachedGoal(RVO::RVOSimulator *sim) {
    /* Check if all agents have reached their goals. */
    for (size_t i = 0; i < sim->getNumAgents(); ++i) {
        if (!sim->isAgentReachGoal(i, goal_small_dist)) {
            return false;
        }
    }
    return true;
}

void addAgent(RVO::RVOSimulator *sim) {
    size_t agentNo = 0;

    float rand_x = rand() % 100 - 50;
    float rand_y = rand() % 100 - 50;

    agentNo = sim->addAgent(RVO::Vector2(rand_x, rand_y));

    float rand_goal_x = rand() % 100 - 50;
    float rand_goal_y = rand() % 100 - 50;

    sim->setAgentGoal(agentNo, rand_goal_x, rand_goal_y);
}

void removeAgent(RVO::RVOSimulator *sim) {
    auto agentNum = sim->getNumAgents();
    if (agentNum == 0) {
        return;
    }
    int rand_i = rand() % agentNum;
    sim->removeAgent(rand_i);
}

void restartSim(RVO::RVOSimulator *sim) {
    sim->removeAllAgent();
    sim->removeAllObstacle();
    setupScenario(sim);
}


void UpdateUI(RVO::RVOSimulator *sim, const std::chrono::duration<double> &frameTime) {
    ImGui::Begin("Tool", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                                  ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SetWindowPos(ImVec2(0, 0));

    if(ImGui::Combo("Scenario", &g_curScenario, items, IM_ARRAYSIZE(items))){
        restartSim(sim);
    }

    if (ImGui::Button("Restart")) {
        restartSim(sim);
    }

    if (ImGui::Button("Add Agent")) {
        addAgent(sim);
    }
    if (ImGui::Button("Remove Agent")) {
        removeAgent(sim);
    }
	if (ImGui::Button("Reset camera")) {
		g_camera.m_center.SetZero();
		g_camera.m_zoom = 1;
	}
	float val = g_pfs.get_unit_move_speed();
	if (ImGui::SliderFloat("unit speed", &val, 0.05, 0.3)) {
		g_pfs.set_unit_move_speed(val);
	}

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(10, ImGui::GetIO().DisplaySize.y - 10), ImGuiCond_Always, ImVec2(0, 1));
    ImGui::SetNextWindowBgAlpha(0.3f);
    ImGui::Begin("Status", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoNav);

    ImGui::SetCursorPos(ImVec2(float(5), float(5)));
    char buffer[128];
    sprintf(buffer, "%.1f ms", 1000.0 * frameTime.count());
    ImGui::TextColored(ImColor(230, 153, 153, 255), buffer);
    ImGui::End();

//        ImGui::ShowStyleEditor();
//        ImGui::ShowDemoWindow();
}

void UpdateImGUI(RVO::RVOSimulator *sim, const std::chrono::duration<double> &frameTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    UpdateUI(sim, frameTime);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UpdateSim(RVO::RVOSimulator *sim) {
    if (!reachedGoal(sim)) {
        setPreferredVelocities(sim);
        sim->doStep();
    }

    for (size_t i = 0; i < sim->getNumAgents(); ++i) {
        auto pos = sim->getAgentPosition(i);
        auto velocity = sim->getAgentVelocity(i);
        g_debugDraw.DrawSolidCircle(b2Vec2(pos.x(), pos.y()), sim->getAgentRadius(i),
                                    b2Vec2(velocity.x(), velocity.y()), b2Color(1, 0, 0));
    }

    auto obstacles = sim->getObstaclesPts();
    for (auto &obstacle : obstacles) {
		b2Vec2* vertices = new b2Vec2[obstacle.size()];
        for (size_t j = 0; j < obstacle.size(); j++) {
            vertices[j].x = obstacle[j].x();
            vertices[j].y = obstacle[j].y();
        }
        g_debugDraw.DrawPolygon(vertices, obstacle.size(), b2Color(0, 1, 0));
		delete[] vertices;
    }


}

void UpdateAstar(){
    if(g_curScenario != 3){
        return;
    }

	// grid
    b2Color grid_color(0.5,0.5,0.5);

	g_debugDraw.DrawSegment(b2Vec2(-100, 0), b2Vec2(100, 0), b2Color(0, 0.7, 0));
    g_debugDraw.DrawSegment(b2Vec2(0,-100),b2Vec2(0,100), b2Color(0, 0.7, 0));

	for (int i = -100; i <= 100; i++) {
		if (i == 0) {
			continue;
		}
		g_debugDraw.DrawSegment(b2Vec2(-100, i), b2Vec2(100, i), grid_color);
		g_debugDraw.DrawSegment(b2Vec2(i, -100), b2Vec2(i, 100), grid_color);
	}

	// blocks
	auto grid_w = g_pfs.get_grid_w();
	auto grid_h = g_pfs.get_grid_h();
	g_debugDraw.DrawSegment(b2Vec2(0, grid_h), b2Vec2(grid_w, grid_h), b2Color(1, 1, 1));
	g_debugDraw.DrawSegment(b2Vec2(grid_w, 0), b2Vec2(grid_w, grid_h), b2Color(1, 1, 1));
	g_debugDraw.DrawSegment(b2Vec2(0, 0), b2Vec2(0, grid_h), b2Color(1, 1, 1));
	g_debugDraw.DrawSegment(b2Vec2(0, 0), b2Vec2(grid_w, 0), b2Color(1, 1, 1));
	auto map = g_pfs.get_map();
	for (int i = 0; i < grid_w; i++) {
		for(int j=0;j< grid_h;j++){
			if (g_pfs.is_block(i, j)) {
				g_debugDraw.DrawSolidRect(b2Vec2(i, j), b2Vec2(i + 1, j + 1), b2Color(1, 1, 1));
			}
		}
	}

	// draw path
	if (!g_pfs.Is_reach_target_pos()) {
		auto path = g_pfs.get_path();
		for (int i = 1; i < path.size();i++) {
			g_debugDraw.DrawSegment(b2Vec2(path[i-1].x, path[i - 1].y), 
				b2Vec2(path[i].x, path[i].y), b2Color(1, 1, 1));
		}
	}
	
	// target pos cross
	auto target_pos = g_pfs.get_target_pos();
	g_debugDraw.DrawSegment(target_pos +b2Vec2(-0.2, 0),target_pos + b2Vec2(0.2, 0), b2Color(1.0, 0, 0));
	g_debugDraw.DrawSegment(target_pos + b2Vec2(0, -0.2),target_pos + b2Vec2(0, 0.2), b2Color(1.0, 0, 0));

	// unit move
	g_pfs.update();

	// draw unit
	g_debugDraw.DrawCircle(g_pfs.get_unit_pos(), g_pfs.get_unit_radius(), b2Color(1, 0, 0));
}

int Init() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (glfwInit() == 0) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    g_mainWindow = glfwCreateWindow(g_camera.m_width, g_camera.m_height, "testbed", NULL, NULL);
    if (g_mainWindow == NULL) {
        fprintf(stderr, "Failed to open GLFW g_mainWindow.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_mainWindow);

    // Load OpenGL functions using glad
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSetScrollCallback(g_mainWindow, ScrollCallback);
    glfwSetWindowSizeCallback(g_mainWindow, ResizeWindowCallback);
    glfwSetKeyCallback(g_mainWindow, KeyCallback);
    glfwSetCharCallback(g_mainWindow, CharCallback);
    glfwSetMouseButtonCallback(g_mainWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(g_mainWindow, MouseMotionCallback);
    glfwSetScrollCallback(g_mainWindow, ScrollCallback);

    g_debugDraw.Create();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    bool success;
    success = ImGui_ImplGlfw_InitForOpenGL(g_mainWindow, false);
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

    return 0;
}

void Update(RVO::RVOSimulator *sim) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    std::chrono::duration<double> frameTime(0.0);
    std::chrono::duration<double> sleepAdjust(0.0);

    while (!glfwWindowShouldClose(g_mainWindow)) {
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        glfwGetWindowSize(g_mainWindow, &g_camera.m_width, &g_camera.m_height);
        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(g_mainWindow, &bufferWidth, &bufferHeight);
        glViewport(0, 0, bufferWidth, bufferHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        UpdateSim(sim);

        UpdateAstar();

        UpdateImGUI(sim, frameTime);


        g_debugDraw.Flush();

        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();

        // Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
        // using mm_pause or equivalent for the last millisecond.
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        std::chrono::duration<double> target(1.0 / 60.0);
        std::chrono::duration<double> timeUsed = t2 - t1;
        std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
        if (sleepTime > std::chrono::duration<double>(0)) {
            std::this_thread::sleep_for(sleepTime);
        }

        std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
        frameTime = t3 - t1;

        // Compute the sleep adjustment using a low pass filter
        sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);
    }
}

void Destroy() {
    g_debugDraw.Destroy();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
}

int main() {
//#if defined(_WIN32)
//    // Enable memory-leak reports
//    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
//#endif

    auto *sim = new RVO::RVOSimulator();
    sim->setTimeStep(1.0f);
    sim->setAgentDefaults(15.0f, 10, 15.0f, 15.0f, 2.0f, 2.0f);
    setupScenario(sim);

    g_camera.m_zoom = 1;
    g_camera.m_center = g_camera_center;
    g_camera.m_width = 1024;
    g_camera.m_height = 768;
    g_camera.m_extent = g_camera_extent;


    if (Init() != 0) {
        return -1;
    }

    Update(sim);

    Destroy();

    return 0;
}

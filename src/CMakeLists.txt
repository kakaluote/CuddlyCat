

set(CuddlyCatExe_SOURCE_FILES
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        main.cpp
        draw.h
        draw.cpp
        b2_math.h
        b2_settings.h
        AStar.cpp
        AStar.h
        util.h
        util.cpp
		PathFindSim.h
		PathFindSim.cpp
		)

set(CuddlyCat_SOURCE_FILES 
	CuddlyCat/App.cpp
	CuddlyCat/App.h
	CuddlyCat/BasicModel.cpp
	CuddlyCat/BasicModel.h
	CuddlyCat/Camera.cpp
	CuddlyCat/Camera.h
	CuddlyCat/const.h
	CuddlyCat/define.h
	CuddlyCat/Director.cpp
	CuddlyCat/Director.h
	CuddlyCat/FileSystem.cpp
	CuddlyCat/FileSystem.h
	CuddlyCat/InputManager.cpp
	CuddlyCat/InputManager.h
	CuddlyCat/Mesh.cpp
	CuddlyCat/Mesh.h
	CuddlyCat/Model.cpp
	CuddlyCat/Model.h
	CuddlyCat/Shader.cpp
	CuddlyCat/Shader.h
	CuddlyCat/ShaderCache.cpp
	CuddlyCat/ShaderCache.h
	CuddlyCat/stb_image.cpp
	CuddlyCat/stb_image.h
	CuddlyCat/Texture.cpp
	CuddlyCat/Texture.h
	CuddlyCat/TextureCache.cpp
	CuddlyCat/TextureCache.h
	CuddlyCat/Timer.cpp
	CuddlyCat/Timer.h
)

set(Demo_SOURCE_FILES
	demo/BaseDemo.cpp
	demo/BaseDemo.h
	demo/Demo.cpp
	demo/Demo.h
	demo/Demo1.cpp
	demo/Demo1.h
	demo/Demo2.cpp
	demo/Demo2.h
	demo/Demo3.cpp
	demo/Demo3.h
	demo/Demo4.cpp
	demo/Demo4.h
	demo/DemoCtrl.cpp
	demo/DemoCtrl.h
)

add_executable(CuddlyCat ${CuddlyCatExe_SOURCE_FILES} ${CuddlyCat_SOURCE_FILES} ${Demo_SOURCE_FILES})
target_include_directories(CuddlyCat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CuddlyCat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/CuddlyCat)
target_link_libraries(CuddlyCat PUBLIC RVO glfw imgui sajson glad ../../lib/assimp-vc141-mtd)

set_target_properties(CuddlyCat PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CuddlyCatExe_SOURCE_FILES} ${CuddlyCat_SOURCE_FILES} ${Demo_SOURCE_FILES})
# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "CuddlyCat" FILES ${CuddlyCat_SOURCE_FILES})
# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "demo" FILES ${Demo_SOURCE_FILES})

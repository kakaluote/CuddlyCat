
cmake_minimum_required(VERSION 2.8)
project(CuddlyCat)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/imgui)
add_subdirectory(extern/sajson)
add_subdirectory(extern/rvo)

add_subdirectory(src)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CuddlyCat)
    set_property(TARGET CuddlyCat PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src")
endif()
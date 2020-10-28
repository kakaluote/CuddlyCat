

set(RVO_HEADERS
	RVO.h
	RVOSimulator.h
	Vector2.h)

set(RVO_SOURCES
	Agent.cpp
	Agent.h
	Definitions.h
	KdTree.cpp
	KdTree.h
	Obstacle.cpp
	Obstacle.h
	RVOSimulator.cpp)

add_library(RVO STATIC ${RVO_HEADERS} ${RVO_SOURCES})
target_include_directories(RVO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RVO PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
		)

if(WIN32)
    set_target_properties(RVO PROPERTIES COMPILE_DEFINITIONS NOMINMAX)
endif()


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${RVO_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "include" FILES ${RVO_HEADERS})
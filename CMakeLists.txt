cmake_minimum_required(VERSION 3.24.2)
project("OctreeExample")
set (CMAKE_CXX_STANDARD 14)

##List of options:
option(BUILD_EXAMPLE "Builds the example" OFF)
option(SILENCE_DEBUG_OUTPUT "Silence debugs" OFF)

include_directories("${CMAKE_CURRENT_LIST_DIR}/Header")
file(GLOB_RECURSE CPPFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp")
file(GLOB_RECURSE HFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Header/*.h")
add_library(Octree ${CPPFiles} ${HFiles})
if(SILENCE_DEBUG_OUTPUT)
	target_compile_definitions(Octree PRIVATE "SILENCE_DEBUG")
endif()
if(BUILD_EXAMPLE)
	add_executable(example "${CMAKE_CURRENT_LIST_DIR}/example_main.cpp")
	target_link_libraries(example PRIVATE Octree)
endif()
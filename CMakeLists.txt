cmake_minimum_required(VERSION 3.12)
project(BeastArena)
include(CMakePrintHelpers)

if(APPLE)
	set(CMAKE_CXX_FLAGS "-I/usr/local/include/SDL2 -D_THREAD_SAFE -L/usr/local/lib -lSDL2")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern/brickengine)
add_subdirectory(include)
add_subdirectory(src)

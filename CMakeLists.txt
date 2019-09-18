cmake_minimum_required(VERSION 3.12)
project(BeastArena)
include(CMakePrintHelpers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern/brickengine)
add_subdirectory(include)
add_subdirectory(src)

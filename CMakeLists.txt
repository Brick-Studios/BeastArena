cmake_minimum_required(VERSION 3.15)

project(BeastArena)

option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json in build directory" ON)

add_subdirectory(extern/brickengine)

add_executable(BeastArena
    ""
)
target_compile_features(BeastArena PUBLIC cxx_std_17)

add_subdirectory(src)


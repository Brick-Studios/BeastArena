cmake_minimum_required(VERSION 3.15)

project(BeastArena)

add_subdirectory(extern/brickengine)

add_executable(BeastArena
    ""
)
target_compile_features(BeastArena PUBLIC cxx_std_17)

add_subdirectory(src)


cmake_minimum_required(VERSION 3.15)

project(BeastArena)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern/brickengine)

add_executable(BeastArena
    ""
)
target_compile_features(BeastArena PUBLIC cxx_std_17)
add_custom_command(TARGET BeastArena PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:BeastArena>/assets)

add_subdirectory(src)


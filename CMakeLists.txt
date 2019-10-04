cmake_minimum_required(VERSION 3.12)
project(BeastArena)
include(CMakePrintHelpers)

if(APPLE)
	set(CMAKE_CXX_FLAGS "-I/usr/local/include/SDL2 -D_THREAD_SAFE -L/usr/local/lib -lSDL2")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
     )

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND /usr/bin/clang-format
    -i
    -style=chromium
    ${ALL_CXX_SOURCE_FILES}
    )
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    clang-tidy
    COMMAND /usr/bin/clang-tidy
    --format-style=chromium
    -checks="cppcoreguidelines-*, modernize-*,"
    -p ./
    ${ALL_CXX_SOURCE_FILES}
    )
endif()

add_subdirectory(extern/brickengine)
add_subdirectory(include)
add_subdirectory(src)

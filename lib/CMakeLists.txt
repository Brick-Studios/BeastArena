file(GLOB_RECURSE BEASTARENALIB_SRC_FILES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)

## Compiler options
# Make the compiler complain, a lot
# Force colors when using ninja with clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
endif()
# Turn on the address sanitizer for safety checks
option(BEASTARENALIB_SANITIZER "Turn on the address sanitizer" OFF)
if(BEASTARENALIB_SANITIZER)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(BeastArenaLib STATIC
    ${BEASTARENALIB_SRC_FILES}
)
target_link_libraries(BeastArenaLib PUBLIC BrickEngine)
target_include_directories(BeastArenaLib PUBLIC ${BEASTARENALIB_INCLUDE_DIRECTORY})

target_compile_features(BeastArenaLib PUBLIC cxx_std_17)

# Export source file list so that it can be used for test compilation
set(BEASTARENALIB_SRC_FILES ${BEASTARENALIB_SRC_FILES} PARENT_SCOPE)
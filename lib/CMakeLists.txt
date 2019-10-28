file(GLOB_RECURSE BEASTARENALIB_SRC_FILES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)


add_library(BeastArenaLib STATIC
    ${BEASTARENALIB_SRC_FILES}
)
## Compiler options
# Make the compiler complain, a lot
target_link_options(BeastArenaLib
    PRIVATE -Wall -Wextra -pedantic -Werror)

target_link_libraries(BeastArenaLib PUBLIC BrickEngine)
target_include_directories(BeastArenaLib PUBLIC ${BEASTARENALIB_INCLUDE_DIRECTORY})

target_compile_features(BeastArenaLib PUBLIC cxx_std_17)

# Export source file list so that it can be used for test compilation
set(BEASTARENALIB_SRC_FILES ${BEASTARENALIB_SRC_FILES} PARENT_SCOPE)
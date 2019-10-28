if (NOT TARGET gtest)
    add_subdirectory("../extern/googletest" "extern/googletest")
endif()

file(GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)
add_executable(BeastArena_Test ${TEST_FILES} ${BEAST_ARENA_SRC_FILES})

## Compiler options
# Make the compiler complain, a lot
target_compile_options(BeastArena_Test
    PRIVATE -Wall -Wextra -pedantic -Werror)
# Turn on the address sanitizer for safety checks
option(BEASTARENA_TEST_SANITIZER "Turn on the address sanitizer" OFF)
if(BEASTARENA_TEST_SANITIZER)
    target_compile_options(BeastArena_Test
        PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(BeastArena_Test
        PRIVATE -fsanitize=address)
endif()
# Linking
target_link_libraries(BeastArena_Test PRIVATE
    BeastArenaLib
    gtest
    gmock
    gtest_main)
target_include_directories(BeastArena_Test PRIVATE ${BEASTARENALIB_INCLUDE_DIRECTORY})

# Adding all the tests
add_test(NAME BeastArena_Test COMMAND BeastArena_Test)
set_target_properties(BeastArena_Test PROPERTIES FOLDER tests)

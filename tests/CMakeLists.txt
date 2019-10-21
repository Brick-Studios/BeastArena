if (NOT TARGET gtest)
    add_subdirectory("../extern/googletest" "extern/googletest")
endif()

file(GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)

add_executable(BeastArena_Test ${TEST_FILES} ${BEAST_ARENA_SRC_FILES})
target_link_libraries(BeastArena_Test PRIVATE
    BeastArenaLib
    gtest
    gmock
    gtest_main)
target_include_directories(BeastArena_Test PRIVATE ${BEASTARENALIB_INCLUDE_DIRECTORY})
add_test(NAME BeastArena_Test COMMAND BeastArena_Test)
set_target_properties(BeastArena_Test PROPERTIES FOLDER tests)

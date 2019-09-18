file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)

add_executable(BeastArena
    ${files}
)
target_link_libraries(BeastArena PRIVATE BrickEngine)
target_include_directories(BeastArena PUBLIC ${INCLUDE_DIRS})
add_custom_command(TARGET BeastArena PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:BeastArena>/assets)
target_compile_features(BeastArena PUBLIC cxx_std_17)

# Export source file list so that it can be used for test compilation
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)
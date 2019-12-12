add_executable(BeastArena
    main.cpp
)
# Make the compiler complain, a lot
target_compile_options(BeastArena
    PRIVATE -Wall -Wextra -pedantic -Werror)

# Turn on the address sanitizer for safety checks
option(BEASTARENA_SANITIZER "Turn on the address sanitizer" OFF)
if(BEASTARENA_SANITIZER)
    target_compile_options(BeastArena
        PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(BeastArena
        PRIVATE -fsanitize=address)
endif()

target_compile_features(BeastArena PUBLIC cxx_std_17)
target_link_libraries(BeastArena PRIVATE BeastArenaLib)

# Custom targets that work on BeastArena
add_custom_target(BeastArena_Resources ALL
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:BeastArena>/assets)

add_executable(acd
    acd_c23.c
    args.c
    spinlock.c
)

target_include_directories(acd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(acd PRIVATE -Wall -Wextra -Wpedantic -Werror -pthread)

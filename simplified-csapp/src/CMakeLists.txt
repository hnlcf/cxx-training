project(
        csapp
        LANGUAGES C
        )

include_directories(
        "."
)

add_library(${PROJECT_NAME} STATIC "")
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wno-unused-label
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wno-unused-but-set-variable
        -fno-common
        -fno-strict-overflow
        -fno-strict-aliasing
        -pthread
        )
target_sources(${PROJECT_NAME} PRIVATE
        csapp/s_alloc.c
        csapp/s_alloc.h
        csapp/s_array.c
        csapp/s_array.h
        csapp/s_common.c
        csapp/s_common.h
        csapp/s_config.h
        csapp/s_cpu.c
        csapp/s_cpu.h
        csapp/s_csapp.h
        csapp/s_instruction.c
        csapp/s_instruction.h
        csapp/s_list.c
        csapp/s_list.h
        csapp/s_log.c
        csapp/s_log.h
        csapp/s_macros.h
        csapp/s_memory.c
        csapp/s_memory.h
        csapp/s_string.c
        csapp/s_string.h
        csapp/s_types.h
        csapp/s_utils.c
        csapp/s_utils.h

        )
target_include_directories(${PROJECT_NAME} PUBLIC
        "."
        )

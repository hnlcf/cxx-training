project(
        simplified-csapp
        VERSION 1.0
        )

enable_testing()
set(CMAKE_C_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(
            S_DEBUG=1
    )
endif ()

add_subdirectory(src)
add_subdirectory(test)

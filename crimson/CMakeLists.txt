project(
        crimson
        VERSION 1.0
        LANGUAGES C CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_C_STANDARD 11)

# Build

## Compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ### Clang
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-lasan)
endif()

## Build main library
add_library(${PROJECT_NAME} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC
        "include"
        )
target_sources(${PROJECT_NAME} PRIVATE
        "src/cn_alloc.c"
        "src/cn_array.c"
        "src/cn_list.c"
        "src/cn_log.c"
        "src/cn_pointer.c"
        "src/cn_sort.c"
        "src/cn_str.c"
        "src/cn_string.c"
        "src/cn_type.c"
        "src/cn_utils.c"
        )

## Build test binary
add_executable(${PROJECT_NAME}_test
        "test/crimson_test.cc"
        "test/cn_array_test.cc"
        "test/cn_list_test.cc"
        "test/cn_pointer_test.cc"
        )
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

# Enable CTest
enable_testing()
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

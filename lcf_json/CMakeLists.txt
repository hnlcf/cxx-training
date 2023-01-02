project(lcf_json C)

set(CMAKE_C_STANDARD 99)

set(
        LIB_SOURCE
        src/lcf_json.c
)
add_library(${PROJECT_NAME} ${LIB_SOURCE})

include_directories(
        include
)
set(
        TEST_SOURCE
        src/test.c
)
add_executable("${PROJECT_NAME}_test" ${TEST_SOURCE})
target_link_libraries("${PROJECT_NAME}_test" lcf_json)

project(csapp)

set(
        CSAPP_SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_alloc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_array.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_cpu.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_instruction.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_log.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_memory.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_string.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/s_utils.c"
)

add_library("${PROJECT_NAME}" ${CSAPP_SRC_FILES})
target_include_directories(
        "${PROJECT_NAME}" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(
        CSAPP_TEST_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_instruction.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_main.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_memory.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_other.cc"
)

add_executable("${PROJECT_NAME}_test" ${CSAPP_TEST_FILES})
target_include_directories(
        "${PROJECT_NAME}_test" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
        "${PROJECT_NAME}_test" PUBLIC
        "${PROJECT_NAME}"
        Catch2::Catch2WithMain
)

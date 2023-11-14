project(crimson)

set(
        CRIMSON_SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_alloc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_array.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_log.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_pointer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_sort.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_str.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_string.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_type.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cn_utils.c"
)

add_library("${PROJECT_NAME}" ${CRIMSON_SRC_FILES})
target_include_directories(
        "${PROJECT_NAME}" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(
        CRIMSON_TEST_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/test/crimson_test.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/cn_array_test.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/cn_list_test.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/cn_pointer_test.cc"
)

add_executable("${PROJECT_NAME}_test" ${CRIMSON_TEST_FILES})
target_include_directories(
        "${PROJECT_NAME}_test" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
        "${PROJECT_NAME}_test" PRIVATE
        "${PROJECT_NAME}"
        Catch2::Catch2WithMain
)

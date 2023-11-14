project(lcf_json)

add_library("${PROJECT_NAME}" "src/lcf_json.c")
target_include_directories(
        "${PROJECT_NAME}" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable("${PROJECT_NAME}_test" "src/test.c")
target_include_directories(
        "${PROJECT_NAME}_test" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
        "${PROJECT_NAME}_test" PRIVATE
        "${PROJECT_NAME}"
        Catch2::Catch2
        Catch2::Catch2WithMain
)

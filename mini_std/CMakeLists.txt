project(mini_std)

set(
        MINISTD_SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/memblock.cpp"

)

add_executable("${PROJECT_NAME}" ${MINISTD_SRC_FILES})
target_include_directories(
        "${PROJECT_NAME}" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

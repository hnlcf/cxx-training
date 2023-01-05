cmake_minimum_required(VERSION 3.23)
project(cxx-training)

set(CMAKE_CXX_STANDARD 20)

# Dependency

## Catch2 for unit test
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(crimson)
add_subdirectory(lcf_json)
add_subdirectory(mini_std)
add_subdirectory(simplified-csapp)
add_subdirectory(test-c)

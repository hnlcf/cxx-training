cmake_minimum_required(VERSION 3.26)

project(cxx-training)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(crimson)
add_subdirectory(lcf_json)
add_subdirectory(mini_std)
add_subdirectory(simplified-csapp)

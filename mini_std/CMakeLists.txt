project(mini_std)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE source_list "src/*.cpp")
file(GLOB_RECURSE header_list "src/*.h")

include_directories(
        "src"
)
add_executable(${PROJECT_NAME} ${source_list} ${header_list})

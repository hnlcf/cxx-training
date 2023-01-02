project(test-c C)

set(CMAKE_C_STANDARD 11)

### Add pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE source_list "src/*.c")
file(GLOB_RECURSE header_list "include/*.h")

include_directories(
        "include")

add_executable(${PROJECT_NAME} ${source_list})

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
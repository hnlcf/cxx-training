project(csapp-test)

include_directories(
        ${CMAKE_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} "")
target_sources(${PROJECT_NAME} PRIVATE
        test_autoheader.h
        test_instruction.cc
        test_main.cc
        test_memory.cc
        test_other.cc
        )
target_link_libraries(${PROJECT_NAME} PRIVATE csapp Catch2::Catch2WithMain)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

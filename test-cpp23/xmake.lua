set_project("test-cpp23")

target("test_cpp23")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("src/*.cppm")
    set_languages("c++20")
    if is_mode("debug") then
        add_defines("DEBUG")
    end
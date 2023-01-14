set_project("test-c")

target("test_c")
    set_kind("binary")
    set_languages("c11")
    add_includedirs("include")
    add_files("src/*.c")
    if is_mode("debug") then
        add_defines("DEBUG")
    end
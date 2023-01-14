set_project("simplified-csapp")

target("csapp")
    set_kind("static")
    set_languages("c11")
    add_includedirs("include", {public = true})
    add_files("src/*.c")
    if is_mode("debug") then
        add_defines("DEBUG")
    end

target("csapp_test")
    set_kind("binary")
    set_languages("c++20")
    add_includedirs("include")
    add_files("test/*.cc")
    add_deps("csapp")
    add_packages("catch2")
    if is_mode("debug") then
        add_defines("DEBUG")
    end

set_project("mini_std")

target("mini_std")
    set_kind("binary")
    set_languages("c++20")
    add_includedirs("src", {public = true})
    add_files("src/*.cpp")
    if is_mode("debug") then
        add_defines("DEBUG")
    end

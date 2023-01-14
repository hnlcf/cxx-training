set_project("lcf_json")

target("lcf_json")
    set_kind("static")
    set_languages("c11")
    add_includedirs("include", {public = true})
    add_files("src/lcf_json.c")
    if is_mode("debug") then
        add_defines("DEBUG")
    end

target("lcf_json_test")
    set_kind("binary")
    set_languages("c11")
    add_includedirs("include")
    add_files("src/test.c")
    add_deps("lcf_json")
    if is_mode("debug") then
        add_defines("DEBUG")
    end

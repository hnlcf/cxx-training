set_project("cxx-training")

add_rules("mode.debug", "mode.release")

add_requires("catch2")

includes("crimson")
includes("lcf_json")
includes("mini_std")
includes("simplified-csapp")
includes("test-c")
includes("test-cpp23")
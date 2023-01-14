-- Project information
set_project("cxx-training")
set_version("1.0.1", { build = "%Y%m%d%H%M" })

-- Configurations
add_rules("mode.debug", "mode.release")
set_config("buildir", "build")
set_config("cc", "gcc")
set_config("ld", "g++")

-- Dependencies
add_requires("catch2")

-- Subprojects
includes("crimson")
includes("lcf_json")
includes("mini_std")
includes("simplified-csapp")
includes("test-c")
includes("test-cpp23")
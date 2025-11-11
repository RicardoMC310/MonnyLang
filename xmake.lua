add_rules("mode.debug", "mode.release")

target("MonnyLang")
    set_kind("binary")
    add_files("src/**.cpp")
    add_includedirs("./include")
    set_optimize("fastest")
add_requires("raylib 5.5")

target("main")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("raylib")
    set_languages("c11", "c++23")
    set_symbols("debug")
    
    set_optimize("fastest")
    
    
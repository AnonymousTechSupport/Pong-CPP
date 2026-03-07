workspace "Engine"
    configurations { "Debug", "Release" }
    location "build"
    architecture "x64"

project "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    toolset "clang"
    
    location "build"
    objdir "build/intermediate"
    targetdir "bin"
    targetname "main"
    
    files {
        "src/**.cpp",
        "src/**.h"
    }
    
    includedirs {
        "src",
        "src/game",
        "src/renderer",
        "src/platform",
        "src/utils",
        "third_party/SDL3-3.4.2/include"
    }
    
    libdirs {
        "third_party/SDL3-3.4.2/lib/x64"
    }
    
    links {
        "SDL3",
        "gdi32",
        "opengl32",
        "user32"
    }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
    
    filter "configurations:Release"
        symbols "Off"
        optimize "On"

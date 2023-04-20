workspace "pc-computer-enhance"
    configurations { "Debug", "Release" }

project "sim8086"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"

    files {
        "%{prj.name}/src/**.cpp",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Speed"
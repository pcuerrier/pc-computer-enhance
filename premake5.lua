workspace "sim8086"
    configurations { "Debug", "Release" }
    architecture "x86_64"

project "sim8086"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x86_64"

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

project "test"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x86_64"

    files {
        "%{wks.name}/test/**.cpp",
    }

    includedirs {
        "vendors/googletest/googletest/include"
    }

    links {
        "vendors/googletest/build/lib/Debug/gtest_main.lib",
        "vendors/googletest/build/lib/Debug/gtest.lib"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Speed"
workspace "Luci-Engine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Luci-Engine/vendor/GLFW/include"

include "Luci-Engine/vendor/GLFW"

project "Luci-Engine"
    location "Luci-Engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "lucipch.h"
    pchsource "%{prj.name}/src/lucipch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "LUCI_PLATFORM_WINDOWS",
            "LUCI_BUILD_DLL"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "LUCI_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "LUCI_RELEASE"
        optimize "On"

    filter "configurations.Dist"
        defines "LUCI_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Luci-Engine/vendor/spdlog/include",
        "Luci-Engine/src"
    }

    links {
        "Luci-Engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "LUCI_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "LUCI_DEBUG"
        symbols "On"
        
    filter "configurations:Release"
        defines "LUCI_RELEASE"
        optimize "On"
    
    filter "configurations.Dist"
        defines "LUCI_DIST"
        optimize "On"

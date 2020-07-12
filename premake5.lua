workspace "Luci-Engine"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Luci-Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Luci-Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Luci-Engine/vendor/imgui"
IncludeDir["glm"] = "Luci-Engine/vendor/glm"

group "Dependencies"
    include "Luci-Engine/vendor/GLFW"
    include "Luci-Engine/vendor/Glad"
    include "Luci-Engine/vendor/imgui"
group ""

project "Luci-Engine"
    location "Luci-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "lucipch.h"
    pchsource "%{prj.name}/src/lucipch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "LUCI_PLATFORM_WINDOWS",
            "LUCI_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "LUCI_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "LUCI_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations.Dist"
        defines "LUCI_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Luci-Engine/vendor/spdlog/include",
        "Luci-Engine/src",
        "Luci-Engine/vendor",
        "%{IncludeDir.glm}"
    }

    links {
        "Luci-Engine"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "LUCI_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "LUCI_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "LUCI_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations.Dist"
        defines "LUCI_DIST"
        runtime "Release"
        optimize "On"

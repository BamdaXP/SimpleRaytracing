project "SimpleRaytracing"
    kind "ConsoleApp"
    language "C++"
    targetdir (outputdir["bin"])
    objdir (outputdir["obj"])

    staticruntime "Off" --Use dynamic CRT

    cppdialect "C++11"
    
    --pchheader "std_pch.hpp"
    --pchsource "src/std_pch.cpp"

    files{
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp",
    }

    includedirs{
        "src/",
        includedir["dependencies"],
    }


    filter "system:windows"
        systemversion "latest"

        defines{
            "SR_PLATFORM_WINDOWS",
            "SR_BUILD_LIB"
        }
    
    filter "configurations:Debug"
        defines "SR_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SR_RELEASE"
        optimize "On"
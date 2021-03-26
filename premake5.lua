workspace "SimpleRaytracing"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

outputdir = {}
outputdir["bin"] = "%{wks.location}/out/bin/%{cfg.buildcfg}/%{cfg.system}_%{cfg.architecture}/%{prj.name}"
outputdir["obj"] = "%{wks.location}/out/obj/%{cfg.buildcfg}/%{cfg.system}_%{cfg.architecture}/%{prj.name}"

includedir = {}
includedir["dependencies"] = "%{wks.location}/dependencies/includes"
includedir["SimpleRaytracing"] = "%{wks.location}/SimpleRaytracing"


libdir = {}
libfile = {}

group ""
    include "SimpleRaytracing"

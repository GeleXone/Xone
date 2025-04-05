workspace "Xone"
    architecture "x64"
    startproject "XoneTest"

    configurations {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Xone"] = {"Xone/include"}

project "Xone"
	location "Xone"
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
	}

    includedirs {
		"%{prj.name}/src",
		"%{prj.name}/include"
	}
	
project "XoneTest"
location "XoneTest"
kind "ConsoleApp"
language "C"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files {
	"%{prj.name}/src/**.h",
	"%{prj.name}/src/**.c",
	"%{prj.name}/src/**.cpp",
}

includedirs {
	"%{prj.name}/src",
	"%{prj.name}/include",
	"Xone/include"
}

links {
	"Xone"
}
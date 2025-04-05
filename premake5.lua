workspace "Xone"
    architecture "x64"
    startproject "example"

    configurations {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Xone"] = {"Xone/include"}

project "Xone"
	location "."
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.c",
		"%{prj.location}/src/**.cpp",
	}

    includedirs {
		"%{prj.location}/include"
	}
	
project "example"
location "example"
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
	"include"
}

links {
	"Xone"
}
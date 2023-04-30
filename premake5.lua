workspace "EDNA"
	architecture "x64"
	startproject "GamePrototype"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directoris relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "EDNA/vendor/GLFW/include"
IncludeDir["Glad"] = "EDNA/vendor/Glad/include"

include "EDNA/vendor/GLFW"
include "EDNA/vendor/Glad"

project "EDNA"
	location "EDNA"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ednapch.h"
	pchsource "EDNA/src/ednapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
				"EDNA_PLATFORM_WINDOWS",
				"EDNA_BUILD_DLL",
				"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/GamePrototype")
		}

		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "EDNA_DIST"
			buildoptions "/MD"
			optimize "On"


project "GamePrototype"
	location "GamePrototype"
	kind "ConsoleApp"
	
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"EDNA/vendor/spdlog/include",
		"EDNA/src"
	}

	links 
	{
		"EDNA"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
				"EDNA_PLATFORM_WINDOWS"
		}


		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "EDNA_DIST"
			buildoptions "/MD"
			optimize "On"


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
IncludeDir["ImGui"] = "EDNA/vendor/imgui"
IncludeDir["glm"] = "EDNA/vendor/glm"

include "EDNA/vendor/GLFW"
include "EDNA/vendor/Glad"
include "EDNA/vendor/imgui"

project "EDNA"
	location "EDNA"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++20"
	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ednapch.h"
	pchsource "EDNA/src/ednapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
				"EDNA_PLATFORM_WINDOWS",
				"EDNA_BUILD_DLL",
				"GLFW_INCLUDE_NONE"
		}

		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "EDNA_DIST"
			runtime "Release"
			optimize "on"


project "GamePrototype"
	location "GamePrototype"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"
	cppdialect "C++20"

	
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
		"EDNA/src",
		"EDNA/vendor",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"EDNA"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
				"EDNA_PLATFORM_WINDOWS"
		}


		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "EDNA_DIST"
			runtime "Release"
			optimize "on"


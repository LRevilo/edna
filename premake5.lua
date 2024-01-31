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
IncludeDir["stb_image"] = "EDNA/vendor/stb_image"
IncludeDir["entt"] = "EDNA/vendor/entt/include"
IncludeDir["OpenAL_Soft"] = "EDNA/vendor/OpenAL-Soft/include"
IncludeDir["libsndfile"] = "EDNA/vendor/libsndfile/include"


-- include projects
include "EDNA/vendor/GLFW"
include "EDNA/vendor/Glad"
include "EDNA/vendor/imgui"
--include "EDNA/vendor/OpenAL-Soft"



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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		--"%{prj.name}/vendor/imgui/**.h",
		--"%{prj.name}/vendor/imgui/**.cpp"


	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.OpenAL_Soft}",
		"%{IncludeDir.libsndfile}",

		--"EDNA/vendor/OpenAL-Soft/include",
		--"EDNA/vendor/OpenAL-Soft/src",
		--"EDNA/vendor/OpenAL-Soft/src/common"

	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"OpenAL32",
		--"OpenAL-Soft",
		"sndfile.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
				"EDNA_PLATFORM_WINDOWS",
				"EDNA_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"AL_LIBTYPE_STATIC"
		}

		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			runtime "Debug"
			symbols "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Debug", "EDNA/vendor/libsndfile/build/Debug"  }

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			runtime "Release"
			optimize "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Release", "EDNA/vendor/libsndfile/build/Release"  }

		filter "configurations:Dist"
			defines "EDNA_DIST"
			runtime "Release"
			optimize "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Release", "EDNA/vendor/libsndfile/build/Release" }


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
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.OpenAL_Soft}",
		"%{IncludeDir.libsndfile}"

	}

	links 
	{
		"EDNA"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
				"EDNA_PLATFORM_WINDOWS",
				"AL_LIBTYPE_STATIC"
		}


		filter "configurations:Debug"
			defines "EDNA_DEBUG"
			runtime "Debug"
			symbols "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Debug", "EDNA/vendor/libsndfile/build/Debug" }

		filter "configurations:Release"
			defines "EDNA_RELEASE"
			runtime "Release"
			optimize "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Release", "EDNA/vendor/libsndfile/build/Release" }

		filter "configurations:Dist"
			defines "EDNA_DIST"
			runtime "Release"
			optimize "on"
			syslibdirs { "EDNA/vendor/OpenAL-Soft/build/Release", "EDNA/vendor/libsndfile/build/Release" }


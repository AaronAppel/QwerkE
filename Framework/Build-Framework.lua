include "Libraries.lua"

project "Framework"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	staticruntime "off" -- https://premake.github.io/docs/staticruntime
	location ""
	defines
	{
		"AssetsDir=\"../Game/Assets/\"",
		"NullAssetsDir=\"../Editor/Assets/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }

	includedirs
	{
		"%{wks.location}/%{prj.name}/Source", -- "Source/QF_PCH.h", -- For precompiled headers to work in VS
		"%{wks.location}/", -- Root for accessing library source
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
		"%{wks.location}/Libraries/freetype2/", -- Required by freetype2
	}
	
	-- disablewarnings { "warnings" }

	forceincludes  { "QF_ForcedIncludes.h", "QF_PCH.h" }
	
	pchheader "QF_PCH.h"
	pchsource "Source/QF_PCH.cpp"

	rtti ("Off")

	targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")
	
	links
	{
		-- Projects
		"cJSON",
		"FlatHeadGames",
		"imgui",
		"lodepng",
		"Mirror",
		
		-- Libraries .lib/.dll
		"%{LibraryLib.assimp}",
		"%{LibraryLib.freetype2}",
		"%{LibraryLib.glew}",
		"%{LibraryLib.glfw}",
		"%{LibraryLib.OpenAL}",
		"opengl32.lib",
		"%{LibraryLib.pthreads}",
	}
	
	vpaths
	{
		-- ["Headers"] = { "**.h", "**.hpp" },
		-- ["Sources/*"] = {"**.c", "**.cpp"},
		-- ["Docs"] = "**.md"
	}
		
	filter "system:windows"
	   systemversion "latest"

	filter "configurations:Debug"
		defines { "_QDebug", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		-- More defines : "QWERKE_TRACKING_ALLOCATIONS", "_QTesting", "_QExample", "_QAdvancedApi", "_QSimpleApi"
		runtime "Debug"
		symbols "On"
		debugargs { }

	filter "configurations:Release"
		defines { "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "On"
		
		links
		{
			"%{LibraryLib.glew}",
			"%{LibraryLib.glfw}",
			"%{LibraryLib.OpenAL}",
			"opengl32.lib",
		}

	filter "configurations:Retail"
		defines { "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
		
		links
		{
			"%{LibraryLib.glew}",
			"%{LibraryLib.glfw}",
			"%{LibraryLib.OpenAL}",
			"opengl32.lib",
		}

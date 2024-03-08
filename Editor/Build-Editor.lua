include "Libraries.lua"

project "Editor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	staticruntime "off"
	location ""
	debugargs { "-applicationName", "Test Game" }
	
	defines
	{	
		-- Path strings
		"AssetsDir=\"Assets/\"",
		-- "NullAssetsDir=AssetsDir",
		"NullAssetsDir=\"A:/GitHub/PremakeProject/Editor/Assets/\"" ,
		
		-- Command line arguments
		"key_StartupDir=\"-startupDir\"",
		"key_ApplicationFileName=\"-appName\"",
		"key_ApplicationName=\"-applicationName\"",
		"key_ProjectFileName=\"-projectFileName\"",
		"key_UserName=\"-userName\"",
	}
	
	debugargs -- User setting changes require VS reload
	{
		"-applicationName", "\"QwerkEditor\"",
		"-projectFileName", "\"Project1\"",
		"-userName", "\"User1\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }

	includedirs
	{
		"%{wks.location}/", -- Root for accessing library source
		"%{wks.location}/Framework/Source/", -- Include Framework source
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
	}
	
	links
	{
		-- Projects
		"FlatHeadGames",
		"Framework",
		"imgui",
		"Mirror",
		
		-- Libraries .lib/.dll
		"opengl32.lib",
	}
	
	forceincludes { "%{wks.location}/Framework/Source/QF_ForcedIncludes.h", "QE_PCH.h" }
	
	pchheader "QE_PCH.h"
	pchsource "Source/QE_PCH.cpp"

	rtti ("Off")
	
	targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"
		defines { }

	filter "configurations:Debug"
		defines { "_QDebug", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Debug"
		optimize "Off"
		symbols "On"
		architecture "x86"

	filter "configurations:Release"
		defines { "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "Off"
		symbols "Off"
		architecture "x86"

	filter "configurations:Retail"
		defines { "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
		architecture "x86"
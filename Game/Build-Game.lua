include "Libraries.lua"

project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	staticruntime "off"
	location ""
	
	defines
	{	
		-- Path strings
		"AssetsDir=\"Assets/\"" ,
		"NullAssetsDir=\"A:/GitHub/PremakeProject/Editor/Assets/\"" ,
		
		-- Command line arguments
		-- #TODO These need to be in sync with Editor. Change that
		"key_StartupDir=\"-startupDir\"",
		"key_ApplicationFileName=\"-appName\"",
		"key_ApplicationName=\"-applicationName\"",
		"key_ProjectFileName=\"-projectFileName\"",
		"key_UserName=\"-userName\"",
	}
	
	debugargs -- User setting changes require VS reload
	{
		"-applicationName", "\"Test Game\"",
		"-projectFileName", "\"Project1\"",
		"-userName", "\"User1\"",
	}
	
	postbuildcommands
	{
		-- Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Scripts/CopyLibraryDLLs.bat \"../Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }

	includedirs
	{
		"%{wks.location}/Editor/Source/", -- Include Editor source
		
		-- #TODO Remove
		"%{wks.location}/Framework/Source/", -- Include Framework source
		"%{wks.location}/", -- Root for accessing library source
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
	}

	links
	{
		"Mirror", -- #TODO Remove
		
		"Framework",
		"Editor",
	}

	-- Linker Additional Options "/NODEFAULTLIB:msvcrt.lib" to disable warning
	-- Also warning LNK4006: __NULL_IMPORT_DESCRIPTOR already defined in glew32d.lib(glew32d.dll); second definition ignored

	forceincludes { "QG_PCH.h" }
	
	pchheader "QG_PCH.h"
	pchsource "Source/QG_PCH.cpp"
	
	rtti ("Off")
	
	targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

	filter "system:windows"
	   systemversion "latest"
	   defines { "WINDOWS" }

	filter "configurations:Debug"
	   defines { "_QDebug", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
	   runtime "Debug"
	   symbols "On"

	filter "configurations:Release"
	   defines { "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
	   runtime "Release"
	   optimize "On"
	   symbols "On"

	filter "configurations:Retail"
	   defines { "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
	   runtime "Release"
	   optimize "On"
	   symbols "Off"
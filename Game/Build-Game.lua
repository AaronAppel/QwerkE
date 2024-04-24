project "Game"
	kind "ConsoleApp"
	location ""
	
	defines
	{
		"WorkspaceRootDir=\"%{wks.location}/\"",
		
		-- Command line arguments
		-- #TODO These need to be in sync with Editor. Change that
		"key_StartupDir=\"-startupDir\"", -- #TODO Change to -executablePath (includes "app.exe")
		"key_ApplicationFileName=\"-appName\"",
		"key_ApplicationName=\"-applicationName\"",
		"key_ProjectFileName=\"-projectFileName\"",
		"key_UserName=\"-userName\"",
		"key_AssetsDirPath=\"-assetsDirPath\"",
		"key_NullAssetsDirPath=\"-nullAssetsDirPath\"",
		
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_FORMAT_MACROS",
	}
	
	debugargs -- #NOTE User setting changes require VS reload
	{
		"-applicationName", "\"Test Game\"",
		"-projectFileName", "\"Project1\"",
		"-userName", "\"User1\"",
		"-assetsDirPath", "\"%{wks.location}%{prj.name}\\Assets\"",
		"-nullAssetsDirPath", "\"%{wks.location}Editor\\Assets\"",
	}
	
	postbuildcommands
	{
		-- Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Scripts/CopyLibraryDLLs.bat \"../Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c", }

	includedirs
	{
		"%{wks.location}/Editor/Source", -- Include Editor source
	}

	links
	{
		"Editor",
	}

	-- Linker Additional Options "/NODEFAULTLIB:msvcrt.lib" to disable warning
	-- ignoredefaultlibraries { "msvcrt" }
	-- Also warning LNK4006: __NULL_IMPORT_DESCRIPTOR already defined in glew32d.lib(glew32d.dll); second definition ignored

	forceincludes { "QG_PCH.h" }
	
	pchheader "QG_PCH.h"
	pchsource "Source/QG_PCH.cpp"

	filter "system:windows"
	   systemversion "latest"
	   defines { "WINDOWS" }

	filter "configurations:Debug"
	   defines { "_QDebug", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }

	filter "configurations:Release"
	   defines { "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }

	filter "configurations:Retail"
	   defines { "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		symbols "off"
		optimize "on"

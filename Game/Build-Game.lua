project "Game"
	kind "ConsoleApp"
	location ""
	
	defines
	{
		"WorkspaceRootDir=\"%{wks.location}/\"",
		
		-- Command line arguments :
		"key_OverrideAssetsDirPath=\"-overrideAssetsDirPath\"",
		"key_OverrideNullAssetsDirPath=\"-overrideNullAssetsDirPath\"",
		
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_FORMAT_MACROS",
	}
	
	debugargs -- #NOTE User setting changes require VS reload
	{
		"-overrideAssetsDirPath", "\"%{wks.location}%{prj.name}\\Assets\"",
		"-overrideNullAssetsDirPath", "\"%{wks.location}Editor\\Assets\"",
	}
	
	postbuildcommands
	{
		-- Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Scripts/CopyLibraryDLLs.bat \"../Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c", }

	includedirs
	{
		"%{wks.location}/Framework/Source",
		"%{wks.location}/", -- Root for accessing library source
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
	}

	links
	{
		"Editor", -- #TODO Remove
		"Framework",
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

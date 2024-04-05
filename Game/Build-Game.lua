project "Game"
	kind "ConsoleApp"
	location ""
	
	defines
	{
		"WorkspaceRootDir=\"%{wks.location}/\"",
		
		-- Command line arguments
		-- #TODO These need to be in sync with Editor. Change that
		"key_StartupDir=\"-startupDir\"",
		"key_ApplicationFileName=\"-appName\"",
		"key_ApplicationName=\"-applicationName\"",
		"key_ProjectFileName=\"-projectFileName\"",
		"key_UserName=\"-userName\"",
		
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_FORMAT_MACROS",
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

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c", }

	includedirs
	{
		"%{wks.location}/Editor/Source", -- Include Editor source
		
		-- Try to remove from Game
		-- "%{wks.location}/Libraries/bx/include", -- Required by bgfx
		-- "%{wks.location}/Libraries/bx/include/compat/msvc", -- Required by bgfx
		-- "%{wks.location}/Libraries/bimg/include", -- Required by bgfx
		-- "%{wks.location}/Libraries/bx/3rdparty", -- Required by bgfx
	}

	links
	{
		"Editor",
	}

	-- Linker Additional Options "/NODEFAULTLIB:msvcrt.lib" to disable warning
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

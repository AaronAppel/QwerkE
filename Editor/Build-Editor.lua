include "Libraries.lua"

project "Editor"
	kind "StaticLib"
	location ""
	debugargs { "-applicationName", "Test Game" }
	
	defines
	{
		"QWERKE_VERSION=0.01",
		"EngineName=\"QwerkE\"",
		"WorkspaceRootDir=\"%{wks.location}/\"",
		
		-- Command line arguments
		"key_StartupDir=\"-startupDir\"",
		"key_ApplicationFileName=\"-appName\"",
		"key_ApplicationName=\"-applicationName\"",
		"key_ProjectFileName=\"-projectFileName\"",
		"key_UserName=\"-userName\"",
		"key_AssetsDirPath=\"-assetsDirPath\"",
		"key_NullAssetsDirPath=\"-nullAssetsDirPath\"",
	}
	
	debugargs -- #NOTE User setting changes require VS reload
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
		
		"%{wks.location}/Libraries/bgfxFramework/include", -- For bgfx helpers
		
		"%{wks.location}/Libraries/bgfx/include", -- For testing calls to bgfx in editor code
		
		"%{wks.location}/Libraries/bx/include", -- To support creating components that depend on bx
		"%{wks.location}/Libraries/bx/include/compat/msvc",
	}
	
	links
	{
		-- Projects
		"Framework",
	}
	
	forceincludes { "%{wks.location}/Framework/Source/QF_ForcedIncludes.h", "QE_PCH.h" }
	
	pchheader "QE_PCH.h"
	pchsource "Source/QE_PCH.cpp"
	
	filter "system:windows"
		systemversion "latest"
		defines { }

	filter "configurations:Debug"
		defines { "BX_CONFIG_DEBUG=1", "_QDebug", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		defines { "BX_CONFIG_DEBUG=0", "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "Off"
		symbols "Off"

	filter "configurations:Retail"
		defines { "BX_CONFIG_DEBUG=0", "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
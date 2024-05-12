include "Libraries.lua"

project "Editor"
	kind "ConsoleApp" -- #TODO Check for non-console app type
	location ""
	rtti "On" -- Enabled for Mirror serialization
	debugargs { "-applicationName", "Test Game" }
	
	defines
	{
		"QWERKE_VERSION=0.01",
		"EngineName=\"QwerkE\"",
		
		-- Command line arguments :
		"key_OverrideAssetsDirPath=\"-overrideAssetsDirPath\"",
		"key_OverrideNullAssetsDirPath=\"-overrideNullAssetsDirPath\"",
	}
	
	debugargs -- #NOTE User setting changes require VS reload
	{
		"-overrideAssetsDirPath", "\"%{wks.location}Assets\"",
		"-overrideNullAssetsDirPath", "\"%{wks.location}Assets\\NullAssets\"",
	}
	
	postbuildcommands
	{
		-- Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Build/CopyLibraryDLLs.bat \"../Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}
	
	files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", "%{prj.name}/**.hpp", "%{prj.name}/**.c" }

	includedirs
	{
		"%{wks.location}/Source", -- Repo root for accessing libraries
		"%{wks.location}/Source/Common/",
		"%{wks.location}/Source/Framework/",
		"%{wks.location}/Source/Libraries/", -- Required by 3rd party libraries
		
		"%{wks.location}/Source/Libraries/bgfxFramework/include", -- For bgfx helpers
		
		"%{wks.location}/Source/Libraries/bgfx/include", -- For testing calls to bgfx in editor code
		
		"%{wks.location}/Source/Libraries/bx/include", -- To support creating components that depend on bx
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
	}
	
	links
	{
		-- Projects
		"Common",
		"Framework",
	}
	
	forceincludes { "QF_ForcedIncludes.h", "QE_PCH.h" }
	
	pchheader "QE_PCH.h"
	pchsource "%{prj.name}/QE_PCH.cpp"

	-- filter "configurations:*32"
	-- 	architecture "x86"
	--	defines { "_Q32BIT", }

	-- filter "configurations:*64"
	-- 	architecture "x86_64"
	--	defines { "_Q64BIT", }
	
	filter "system:windows"
		systemversion "latest"
		defines { }

	filter "configurations:Debug"
		defines { "BX_CONFIG_DEBUG=1", "_QDebug", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		defines { "BX_CONFIG_DEBUG=0", "_QRelease", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "Off"
		symbols "Off"

	filter "configurations:Retail"
		defines { "BX_CONFIG_DEBUG=0", "_QRetail", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
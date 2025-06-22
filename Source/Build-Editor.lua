include "Libraries.lua"

project "Editor"
	kind "ConsoleApp" -- #TODO Check for non-console app type
	location ""
	rtti "On" -- #NOTE Enabled for Mirror serialization
	debugargs { "-applicationName", "Test Game" }
	
	defines
	{
		"QWERKE_VERSION=0.01",
		"EngineName=\"QwerkE\"",
		
		-- #NOTE Command line arguments :
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
		-- #NOTE Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Build/PostBuild.bat \"../Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}
	
	files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", "%{prj.name}/**.hpp", "%{prj.name}/**.c" }

	includedirs
	{
		"%{wks.location}/Source", -- #NOTE Repo root for accessing libraries
		"%{wks.location}/Source/Common/",
		"%{wks.location}/Source/Framework/",
		"%{wks.location}/Source/Libraries/", -- #NOTE Required by 3rd party libraries
		
		"%{wks.location}/Source/Libraries/bgfxFramework/include", -- #NOTE For bgfx helpers
		
		"%{wks.location}/Source/Libraries/bgfx/include", -- #NOTE For testing calls to bgfx in editor code
		
		"%{wks.location}/Source/Libraries/bx/include", -- #NOTE To support creating components that depend on bx
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
		
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links -- Project references
	{
		"Common",
		"Framework",
	}
	
	forceincludes { "QE_PCH.h", "QF_ForcedIncludes.h" }
	
	pchheader "QE_PCH.h"
	pchsource "%{prj.name}/QE_PCH.cpp"

	-- filter "configurations:*32"
	-- 	architecture "x86"
	--	defines { "_Q32BIT", }

	-- filter "configurations:*64"
	-- 	architecture "x86_64"
	--	defines { "_Q64BIT", }
			
	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" } -- bx dependency
	
	filter "system:windows"
		systemversion "latest"
		defines { }

	filter "configurations:Debug"
		defines
		{
			"_QDebug",
			"LibrariesDir=\"%{wks.location}/Libraries/\"",
			LibraryDefines,
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
		}
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		defines
		{
			"_QRelease",
			"LibrariesDir=\"%{wks.location}/Libraries/\"",
			LibraryDefines,
			"BX_CONFIG_DEBUG=0", -- Required by bgfx
		}
		runtime "Release"
		optimize "Off"
		symbols "Off"

	filter "configurations:Retail"
		defines
		{
			"_QRetail",
			"LibrariesDir=\"%{wks.location}/Libraries/\"",
			LibraryDefines,
			"BX_CONFIG_DEBUG=0", -- Required by bgfx
		}
		runtime "Release"
		optimize "On"
		symbols "Off"
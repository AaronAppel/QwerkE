include "Libraries.lua"

project "Framework"
	kind "StaticLib"
	location ""
	rtti "On" -- Enabled for Mirror serialization
	
	local rootDir = "%{wks.location}/"
	rootDir = rootDir:gsub("/", "\\") -- Replace '/' with '\\' for paths
	
	defines
	{
		-- #TODO Deprecate WorkspaceRootDir
		"WorkspaceRootDir=\"" .. "%{wks.location}/" .. "\"",
		
		"EngineName=\"QwerkE\"",
		"key_ExePath=\"key_ExePath\"",
		
		-- Command line arguments :
		"key_OverrideAssetsDirPath=\"-overrideAssetsDirPath\"",
		"key_OverrideNullAssetsDirPath=\"-overrideNullAssetsDirPath\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }
	excludes { "Source/QF_RendererBgfxExample_minimal.cpp", }

	includedirs
	{
		"%{wks.location}/Common/Source/",
		
		"%{wks.location}/%{prj.name}/Source", -- "Source/QF_PCH.h", -- For precompiled headers to work in VS
		"%{wks.location}/", -- Root for #including "Libraries/..."
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
		
		"%{wks.location}/Libraries/bgfx/3rdparty",
		"%{wks.location}/Libraries/bgfx/include",
		"%{wks.location}/Libraries/bgfxFramework/include",
		"%{wks.location}/Libraries/bimg/include",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
	}
	
	-- disablewarnings { "warnings" }

	forceincludes  { "QF_ForcedIncludes.h", "QF_PCH.h" }
	
	pchheader "QF_PCH.h"
	pchsource "Source/QF_PCH.cpp"

	links
	{
		-- Projects
		"cJSON",
		"bgfxFramework",
		"FlatHeadGames",
		"imgui",
		"imgui-node-editor",
		"Jolt",
		"lodepng",
		"Mirror",
		"spdlog",
		
		"Common",
		
		-- Libraries .lib/.dll
		"%{LibraryLib.glfw}",
	}
	
	vpaths -- VS Filters
	{
		-- ["Headers"] = { "**.h", "**.hpp" },
		-- ["Sources/*"] = {"**.c", "**.cpp"},
		-- ["Docs"] = "**.md"
	}
		
	filter "system:windows"
		links { "psapi", } -- bgfx requires psapi
		systemversion "latest"

	-- filter "configurations:*32"
	-- 	architecture "x86"
	--	defines { "_Q32BIT", }

	-- filter "configurations:*64"
	-- 	architecture "x86_64"
	--	defines { "_Q64BIT", }

	filter "configurations:Debug"
		defines
		{
			"_QDEBUG",
			"LibrariesDir=\"%{wks.location}/Libraries/\"", -- #TODO Remove
			LibraryDefines, 
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
		}
		-- More defines : "QWERKE_TRACKING_ALLOCATIONS", "_QTesting", "_QExample", "_QAdvancedApi", "_QSimpleApi"
		runtime "Debug"
		symbols "On"
		debugargs { }

	filter "configurations:Release"
		defines { "_QRELEASE", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Retail"
		defines { "_QRETAIL", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
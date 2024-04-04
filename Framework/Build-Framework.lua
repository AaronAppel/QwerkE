include "Libraries.lua"

project "Framework"
	kind "StaticLib"
	location ""
	
	defines
	{
		-- "AssetsDir=\"%{wks.location}/Editor/Assets/\"" ,
		-- "NullAssetsDir=\"%{wks.location}/Editor/Assets/\"" ,
		"WorkspaceRootDir=\"%{wks.location}/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }
	excludes { "Source/QF_RendererBgfxExample_minimal.cpp", }

	includedirs
	{
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
		"lodepng",
		"Mirror",
		"spdlog",
		
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

	filter "configurations:Debug"
		defines
		{
			"_QDEBUG",
			"_Q32Bit",
			"LibrariesDir=\"%{wks.location}/Libraries/\"",
			LibraryDefines, 
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
		}
		-- More defines : "QWERKE_TRACKING_ALLOCATIONS", "_QTesting", "_QExample", "_QAdvancedApi", "_QSimpleApi"
		runtime "Debug"
		symbols "On"
		debugargs { }

	filter "configurations:Release"
		defines { "_QRELEASE", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Retail"
		defines { "_QRETAIL", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"
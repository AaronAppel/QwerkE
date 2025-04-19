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

	files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", "%{prj.name}/**.hpp", "%{prj.name}/**.c" }
	excludes { "%{prj.name}/QF_RendererBgfxExample_minimal.cpp", }

	includedirs
	{
		"%{wks.location}/Source/Common/",
		
		"%{wks.location}/Source/%{prj.name}", -- "Source/QF_PCH.h", -- For precompiled headers to work in VS
		"%{wks.location}/Source/", -- Root for #including "Libraries/..."
		"%{wks.location}/Source/Libraries/", -- Required by 3rd party libraries
		
		"%{wks.location}/Source/Libraries/bgfx/3rdparty",
		"%{wks.location}/Source/Libraries/bgfx/include",
		"%{wks.location}/Source/Libraries/bgfxFramework/include",
		"%{wks.location}/Source/Libraries/bimg/include",
		"%{wks.location}/Source/Libraries/bx/include",
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Source/Libraries/imgui", -- Required by bgfx
	}

	forceincludes  { "QF_PCH.h", "QF_ForcedIncludes.h"  }
	
	pchheader "QF_PCH.h"
	pchsource "%{prj.name}/QF_PCH.cpp"
	
	-- #TODO Setup warning and error levels to a useful standard and clean through all issues
	-- warnings "Extra"
	-- filter { "toolset:msc" }
		-- fatalwarnings { "2001" }
	
	links -- Project references
	{
		"cJSON",
		"bgfxFramework",
		"FlatHeadGames",
		"ImFileDialog",
		"ImCoolBar",
		"imgui",
		"imgui_hex_editor",
		"imgui_toggle",
		"imgui-console",
		"imgui-spin-value",
		"imgui-knobs",
		"imgui-node-editor",
		"ImGuizmo",
		"im-neo-sequencer",
		"ImNodeFlow",
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
			"LibrariesDir=\"%{wks.location}/Source/Libraries/\"", -- #TODO Remove
			LibraryDefines, 
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
		}
		-- More defines : "QWERKE_TRACKING_ALLOCATIONS", "_QTesting", "_QExample", "_QAdvancedApi", "_QSimpleApi"
		runtime "Debug"
		symbols "On"
		debugargs { }

	filter "configurations:Release"
		defines { "_QRELEASE", "LibrariesDir=\"%{wks.location}/Source/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Retail"
		defines { "_QRETAIL", "LibrariesDir=\"%{wks.location}/Source/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"

include "solution_items.lua"

workspace "QwerkE Game"
	configurations { "Debug", "Release", "Retail" }
	-- solution_items { ".editorconfig" }
	startproject "Game"
	location "../"
	flags { "MultiProcessorCompile" }

	-- filter "configurations:*86"
		-- architecture "x86"
	-- filter "configurations:*64"
		-- architecture "x64"
		
	filter "configurations:*"
		architecture "x86"
	
	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

	filter "system:windows"
		buildoptions { "/Zc:__cplusplus" }
		
	-- Keep defines above includes, or else they may not get included by projects inlcuded below
	defines { "QWERKE_VERSION=0.01", "_QWINDOWS", "WIN32_LEAN_AND_MEAN", "EngineName=\"QwerkE\"", }
	
	group "Dependencies"
	include "../Libraries/bgfx/Build-bgfx.lua"
	include "../Libraries/bx/Build-bx.lua"
	include "../Libraries/cJSON/Build-cJSON.lua"
	include "../Libraries/FlatHeadGames/Build-FlatHeadGames.lua"
	include "../Libraries/imgui/Build-imgui.lua"
	include "../Libraries/lodepng/Build-lodepng.lua"
	include "../Libraries/Mirror/Build-Mirror.lua"
	group ""
	
	-- buildoptions { "/EHsc", "/Zc:preprocessor", } -- bx doesn't like "/EHsc", "/Zc:preprocessor"

	include "../Framework/Build-Framework.lua"
	include "../Editor/Build-Editor.lua"
	include "../Game/Build-Game.lua"

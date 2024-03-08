include "solution_items.lua"

workspace "QwerkE Game"
	configurations { "Debug", "Release", "Retail" }
	-- solution_items { ".editorconfig" }
	startproject "Game"
	location "../"
	defines { "QWERKE_VERSION=0.01", "_QWINDOWS", "WIN32_LEAN_AND_MEAN", "EngineName=\"QwerkE\"", }
	flags { "MultiProcessorCompile" }

	-- filter "configurations:*86"
		-- architecture "x86"
	-- filter "configurations:*64"
		-- architecture "x64"
		
	filter "configurations:*"
		architecture "x86"
	
	filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
	
	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

	group "Dependencies"
	include "../Libraries/cJSON/Build-cJSON.lua"
	include "../Libraries/FlatHeadGames/Build-FlatHeadGames.lua"
	include "../Libraries/imgui/Build-imgui.lua"
	include "../Libraries/lodepng/Build-lodepng.lua"
	include "../Libraries/Mirror/Build-Mirror.lua"
	group ""

	include "../Framework/Build-Framework.lua"
	include "../Editor/Build-Editor.lua"
	include "../Game/Build-Game.lua"

workspace "QwerkE Game"

	-- Workspace specific settings
	startproject "Game"
	location "../"

	-- Solution wide shared settings
	configurations { "Debug", "Release", "Retail" }
	flags { "MultiProcessorCompile" }
	rtti "Off"
	staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++17"

	-- filter "configurations:*32"
	-- 	architecture "x86"
	-- filter "configurations:*64"
	-- 	architecture "x86_64"
	
	filter "configurations:*"
		architecture "x64"
		
	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin/int/" .. OutputDir .. "/%{prj.name}")

	filter "system:windows"
		buildoptions { "/Zc:__cplusplus" }
		systemversion "latest"
		defines { "_QWINDOWS", "WIN32_LEAN_AND_MEAN", }
		
	filter "configurations:Debug"
		defines { "_QDEBUG", "WIN32_LEAN_AND_MEAN", } -- #TODO _QDEBUG should be all caps to match convention
		runtime "Debug"
		symbols "on"
		optimize "off"
		
	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"
	
	group "Dependencies"
	include "../Libraries/bgfx/Build-bgfx.lua"
	include "../Libraries/bgfxFramework/Build-bgfxFramework.lua"
	include "../Libraries/bimg/Build-bimg.lua"
	include "../Libraries/bx/Build-bx.lua"
	include "../Libraries/cJSON/Build-cJSON.lua"
	include "../Libraries/FlatHeadGames/Build-FlatHeadGames.lua"
	include "../Libraries/imgui/Build-imgui.lua"
	include "../Libraries/lodepng/Build-lodepng.lua"
	include "../Libraries/Mirror/Build-Mirror.lua"
	include "../Libraries/spdlog/Build-spdlog.lua"
	group ""
	
	-- buildoptions { "/EHsc", "/Zc:preprocessor", } -- bx doesn't like "/EHsc", "/Zc:preprocessor"

	include "../Framework/Build-Framework.lua"
	include "../Editor/Build-Editor.lua"
	include "../Game/Build-Game.lua"

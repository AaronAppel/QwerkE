workspace "QwerkE"

	-- Workspace specific settings
	startproject "Editor"
	location "../"

	-- Solution wide shared settings
	configurations { "Debug", "Release", "Retail" }
	flags { "MultiProcessorCompile" }
	rtti "Off" -- #NOTE Needed for Mirror projects (Editor, Framework)
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

	filter "system:windows" -- #TODO Review moving defines lower, below Dependencies
		buildoptions { "/Zc:__cplusplus" }
		systemversion "latest"
		defines { "_QWINDOWS", "WIN32_LEAN_AND_MEAN", }
		
	filter "configurations:Debug"
		defines { "_QDEBUG", "WIN32_LEAN_AND_MEAN", }
		runtime "Debug"
		symbols "on"
		optimize "off"
		
	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"
	
	group "Dependencies"
	include "../Source/Libraries/bgfx/Build-bgfx.lua"
	include "../Source/Libraries/bgfxFramework/Build-bgfxFramework.lua"
	include "../Source/Libraries/bimg/Build-bimg.lua"
	include "../Source/Libraries/bx/Build-bx.lua"
	include "../Source/Libraries/cJSON/Build-cJSON.lua"
	include "../Source/Libraries/FlatHeadGames/Build-FlatHeadGames.lua"
	include "../Source/Libraries/ImCoolBar/Build-ImCoolBar.lua"
	include "../Source/Libraries/ImFileDialog/Build-ImFileDialog.lua"
	include "../Source/Libraries/imgui/Build-imgui.lua"
	include "../Source/Libraries/imgui_hex_editor/Build-imgui_hex_editor.lua"
	include "../Source/Libraries/imgui_toggle/Build-imgui_toggle.lua"
	include "../Source/Libraries/imgui-console/Build-imgui-console.lua"
	include "../Source/Libraries/imgui-knobs/Build-imgui-knobs.lua"
	include "../Source/Libraries/imgui-node-editor/Build-imgui-node-editor.lua"
	include "../Source/Libraries/imgui-notify/Build-imgui-notify.lua"
	include "../Source/Libraries/imgui-spin-value/Build-imgui-spin-value.lua"
	include "../Source/Libraries/ImGuizmo/Build-ImGuizmo.lua"
	include "../Source/Libraries/im-neo-sequencer/Build-im-neo-sequencer.lua"
	include "../Source/Libraries/ImNodeFlow/Build-ImNodeFlow.lua"
	include "../Source/Libraries/imspinner/Build-imspinner.lua"
	include "../Source/Libraries/Jolt/Build-Jolt.lua"
	include "../Source/Libraries/lodepng/Build-lodepng.lua"
	include "../Source/Libraries/Mirror/Build-Mirror.lua"
	include "../Source/Libraries/spdlog/Build-spdlog.lua"
	include "../Source/Libraries/stb_image/Build-stb_image.lua"
	group ""
	
	-- buildoptions { "/EHsc", "/Zc:preprocessor", } -- bx doesn't like "/EHsc", "/Zc:preprocessor"

	include "../Source/Build-Common.lua"
	include "../Source/Build-Framework.lua"
	include "../Source/Build-Editor.lua"
	include "../Source/Build-Game.lua"

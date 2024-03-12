include "Libraries.lua"

project "Framework"
	kind "StaticLib"
	location ""
	
	defines
	{
		"AssetsDir=\"../Game/Assets/\"",
		"NullAssetsDir=\"../Editor/Assets/\"",
	}

	files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }
	excludes { "Source/QF_RendererBgfxExample_minimal.cpp", }

	includedirs
	{
		"%{wks.location}/%{prj.name}/Source", -- "Source/QF_PCH.h", -- For precompiled headers to work in VS
		"%{wks.location}/", -- Root for accessing library source
		"%{wks.location}/Libraries/", -- Required by 3rd party libraries
		
		-- "%{wks.location}/Libraries/bx/include", -- Required by bgfx
		-- "%{wks.location}/Libraries/bx/include/compat/msvc", -- Required by bgfx
		-- "%{wks.location}/Libraries/bimg/include", -- Required by bgfx
		
		-- Required by bgfx
		-- "%{wks.location}/Libraries/bgfx/include",
		-- "%{wks.location}/Libraries/bgfx/3rdparty",
		-- "%{wks.location}/Libraries/bgfx/3rdparty/directx-headers/include/directx",
		-- "%{wks.location}/Libraries/bgfx/3rdparty/khronos",
		-- "%{wks.location}/Libraries/bx/include",
		-- "%{wks.location}/Libraries/bx/include/compat/msvc",
		-- "%{wks.location}/Libraries/bimg/include",
		
		"%{wks.location}/Libraries/bgfx/include",
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
		"bgfx",
		"FlatHeadGames",
		"imgui",
		"lodepng",
		"Mirror",
		
		-- Libraries .lib/.dll
		"%{LibraryLib.glew}",
		"%{LibraryLib.glfw}",
		"%{LibraryLib.OpenAL}",
		"opengl32.lib",
		"%{LibraryLib.pthreads}",
	}
	
	vpaths -- VS Filters
	{
		-- ["Headers"] = { "**.h", "**.hpp" },
		-- ["Sources/*"] = {"**.c", "**.cpp"},
		-- ["Docs"] = "**.md"
	}
		
	filter "system:windows"
		links { "psapi", "gdi32.lib", }
		systemversion "latest"

	filter "configurations:Debug"
		defines
		{
			"_QDebug",
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
		defines { "_QRelease", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Retail"
		defines { "_QRetail", "_Q32Bit", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		optimize "On"
		symbols "Off"

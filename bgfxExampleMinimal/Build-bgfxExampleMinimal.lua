project "bgfxExampleMinimal"
	kind "StaticLib"
	location ""

	files { "*.h", "*.cpp", }

	includedirs
	{
		"%{wks.location}/Libraries/",
		"%{wks.location}/Libraries/bgfx/include",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
	}
		
	links
	{
		"bgfx",
	}

	filter "system:windows"
		links { "psapi" }
		systemversion "latest"

	filter "configurations:Debug"
		defines
		{
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
		}
		runtime "Debug"
		symbols "On"
		debugargs { }


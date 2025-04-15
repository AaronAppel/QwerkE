project "bgfxFramework"
	kind "StaticLib"
	location ""
	
	files
	{
		"include/bgfxFramework/**.h",
		"src/**.cpp",
	}
	
	excludes
	{
		-- "3rdparty/dear-imgui/*.inl", -- #TODO Review
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/%{prj.name}/include/bgfxFramework",
		
		"%{wks.location}/Source/Libraries/bgfx/3rdparty",
		"%{wks.location}/Source/Libraries/bgfx/include",
		"%{wks.location}/Source/Libraries/bx/include",
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Source/Libraries/bimg/include",
		"%{wks.location}/Source/Libraries/include",
		
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"bgfx",
	}

	filter "configurations:Debug"
		defines
		{
			"__STDC_FORMAT_MACROS",
			"_CRT_SECURE_NO_WARNINGS",
			"_HAS_EXCEPTIONS=0",
			"BX_CONFIG_DEBUG=1",
		}
		
	filter "configurations:Release"
		defines
		{
			"__STDC_FORMAT_MACROS",
			"_CRT_SECURE_NO_WARNINGS",
			"_HAS_EXCEPTIONS=0",
			"BX_CONFIG_DEBUG=0",
		}

project "bgfx"
	kind "StaticLib"
	location ""
	
	files
	{
		"include/bgfx/**.h",
		"src/*.cpp",
		"src/*.h",
		"3rdparty/meshoptimizer/src/*codec.cpp",
		"3rdparty/dear-imgui/*.h",
		"3rdparty/dear-imgui/*.cpp",
		"3rdparty/dear-imgui/*.inl",
	}
	
	excludes
	{
		"src/amalgamated.cpp",
	}

	includedirs
	{
		"include",
		"3rdparty",
		"3rdparty/directx-headers/include/directx",
		"3rdparty/dxsdk/include",
		"3rdparty/khronos",
		
		"%{wks.location}/Source/Libraries/bx/include",
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Source/Libraries/bimg/include",
	}
	
	links
	{
		"bimg",
		"bx",
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

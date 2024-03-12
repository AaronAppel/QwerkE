project "bgfx"
	kind "StaticLib"
	location ""
	
	files
	{
		"include/bgfx/**.h",
		"src/*.cpp",
		"src/*.h",
	}
	
	excludes
	{
		"src/amalgamated.cpp",
	}

	includedirs
	{
		-- "%{wks.location}/Libraries/%{prj.name}/include",
		-- "%{wks.location}/Libraries/%{prj.name}/3rdparty",
		-- "%{wks.location}/Libraries/%{prj.name}/3rdparty/khronos",
		-- "%{wks.location}/Libraries/bx/include",
		-- "%{wks.location}/Libraries/bx/include/compat/msvc",
		-- "%{wks.location}/Libraries/bimg/include",
		
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Libraries/bimg/include",
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/directx-headers/include/directx",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/dxsdk/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/khronos",
	}

	defines
	{
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_LIMIT_MACROS",
		-- "__STDC_FORMAT_MACROS",
		-- "__STDC_CONSTANT_MACROS",
		-- "_DEBUG",
		-- "WIN32",
		-- "_WIN32",
		-- "_HAS_EXCEPTIONS=0",
		-- "_SCL_SECURE=0",
		-- "_SECURE_SCL=0",
		-- "_SCL_SECURE_NO_WARNINGS",
		-- "_CRT_SECURE_NO_WARNINGS",
		-- "_CRT_SECURE_NO_DEPRECATE",
		
		"_DEBUG",
		"BX_CONFIG_DEBUG=1",
		"__STDC_FORMAT_MACROS",
		"_CRT_SECURE_NO_WARNINGS",
		"_HAS_EXCEPTIONS=0",
	}
	
	links
	{
		"bimg",
		"bx",
	}

	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
		
	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"

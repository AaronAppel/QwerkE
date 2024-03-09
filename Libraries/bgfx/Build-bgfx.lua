project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	location ""
	staticruntime "off"

	targetdir ("../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../bin/int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"include/bgfx/*.h",
		"include/bgfx/*.cpp",
		"src/*.h",
		"src/*.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/directx-headers/include/directx",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/khronos",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Libraries/bimg/include",
	}

	defines
	{
		"BX_CONFIG_DEBUG=1",
		"__STDC_LIMIT_MACROS",
		"__STDC_FORMAT_MACROS",
		"__STDC_CONSTANT_MACROS",
		"_DEBUG",
		"WIN32",
		"_WIN32",
		"_HAS_EXCEPTIONS=0",
		"_SCL_SECURE=0",
		"_SECURE_SCL=0",
		"_SCL_SECURE_NO_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_SECURE_NO_DEPRECATE",
	}
	
	links
	{
		-- "bx",
	}
		
	filter "configurations:*"
		architecture "x86"
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"

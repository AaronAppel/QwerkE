project "bx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	location ""
	staticruntime "off"

	targetdir ("../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../bin/int/" .. OutputDir .. "/%{prj.name}")

	includedirs
	{
		"%{wks.location}/Libraries/%{prj.name}/3rdparty", -- bx requirement
		"%{wks.location}/Libraries/%{prj.name}/include", -- bx requirement
		"%{wks.location}/Libraries/%{prj.name}/include/compat/msvc", -- bx requirement
	}
	
	rtti ("Off")
	
	nativewchar "On"
	
	files
	{
		"**.h",
		"**.cpp",
		"**.inl",
		-- "src/amalgamated.cpp",
	}
	
	includedirs	{ }

	buildoptions
	{
		"/wd4201",
		"/wd4324",
		"/Ob2",
	}

	defines
	{
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
		"BX_CONFIG_DEBUG=1",
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

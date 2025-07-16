project "openal-soft"
	kind "StaticLib"
	location ""
	-- staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++20"
	
	filter "configurations:*"
		architecture "x64"
	
	files
	{
		"**.h",
		"**.cpp",
		"**.txt",
	}
	
	excludes
	{
		""
	}

	includedirs
	{
		"include",
		"build",
		"common",
		"core",
		"gsl/include",
		"fmt-11.1.4/include",
		
		"%{wks.location}/Source/Libraries/openal-soft",
		
		"%{wks.location}/Source/Libraries"
	}
	
	links
	{
	}
	
	filter "action:vs*"
		buildoptions { "/w14640 /utf-8"}

	filter "configurations:Debug"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Debug\"",
		}
		
	filter "configurations:Release"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"NDEBUG",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Release\"",
		}
		
	filter "configurations:Retail"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"NDEBUG",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Release\"",
		}

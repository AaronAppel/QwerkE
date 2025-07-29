project "openal-softfmt"
	kind "StaticLib"
	location ""
	-- staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++20"
	
	filter "configurations:*"
		architecture "x64"
	
	files
	{
		"fmt-11.1.4/include/fmt/args.h",
		"fmt-11.1.4/include/fmt/base.h",
		"fmt-11.1.4/include/fmt/chrono.h",
		"fmt-11.1.4/include/fmt/color.h",
		"fmt-11.1.4/include/fmt/compile.h",
		"fmt-11.1.4/include/fmt/core.h",
		"fmt-11.1.4/include/fmt/format.h",
		"fmt-11.1.4/include/fmt/format-inl.h",
		"fmt-11.1.4/include/fmt/os.h",
		"fmt-11.1.4/include/fmt/ostream.h",
		"fmt-11.1.4/include/fmt/printf.h",
		"fmt-11.1.4/include/fmt/ranges.h",
		"fmt-11.1.4/include/fmt/std.h",
		"fmt-11.1.4/include/fmt/xchar.h",
		
		"fmt-11.1.4/src/format.cc",		
		"fmt-11.1.4/src/os.cc",
	}
	
	removefiles -- excludes
	{
	}

	includedirs
	{
		"fmt-11.1.4/include",
	}
	
	links
	{
	}
	
	filter "action:vs*"
		buildoptions { "/utf-8 /machine:x64"}

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

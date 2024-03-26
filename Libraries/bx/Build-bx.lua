project "bx"
	kind "StaticLib"
	location ""

	files
	{
		"include/%{prj.name}/*.h",
		"include/%{prj.name}/inline/*.inl",
		"src/*.cpp"
	}
	
	excludes
	{
		"src/amalgamated.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/include/compat/msvc",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
	}
	
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "configurations:Debug"
		defines
		{
			"__STDC_FORMAT_MACROS",
			-- "_CRT_SECURE_NO_WARNINGS",
			"_HAS_EXCEPTIONS=0",
			"BX_CONFIG_DEBUG=1",
		}
		
	filter "configurations:Release"
		defines
		{
			"__STDC_FORMAT_MACROS",
			"_HAS_EXCEPTIONS=0",
			"BX_CONFIG_DEBUG=0",
		}
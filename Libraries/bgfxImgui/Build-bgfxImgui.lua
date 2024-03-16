project "bgfxImgui"
	kind "StaticLib"
	location ""
	
	files
	{
		"**.h",
		"**.cpp",
	}
	
	excludes { }

	includedirs
	{
		"%{wks.location}/Libraries/%{prj.name}",
		"%{wks.location}/Libraries/%{prj.name}/stb",
		"%{wks.location}/Libraries/bgfx/include",
		"%{wks.location}/Libraries/bimg/include",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
	}

	defines
	{	
		"__STDC_FORMAT_MACROS",
	}
	
	links
	{
		"bx",
	}

	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"

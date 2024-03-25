project "bimg"
	kind "StaticLib"
	location ""

	files
	{
		"include/bimg/*.h",
		-- "src/image.cpp",
		-- "src/image_gnf.cpp",
		"src/*.h",
		"src/*.cpp",
		"3rdparty/astc-encoder/source/*",
		"3rdparty/tinyexr/deps/miniz/miniz*",
	}

	includedirs
	{		
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-codec",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-codec/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-encoder/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/iqa/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/tinyexr/deps/miniz",
	}

	defines
	{
		"_HAS_EXCEPTIONS=0", -- #TODO Add to target configuration
	}
	
	filter "configurations:Debug"
		defines { "BX_CONFIG_DEBUG=1", "_DEBUG", }
		
	filter "configurations:Release"
		defines { "BX_CONFIG_DEBUG=0", }

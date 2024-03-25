project "spdlog"
	kind "StaticLib"
	location ""
	
	files
	{
		"**.h",
		"**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/",
	}

	defines { }

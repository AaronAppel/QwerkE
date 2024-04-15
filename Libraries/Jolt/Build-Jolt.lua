project "Jolt"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp",
	}

	includedirs
	{		
		"%{wks.location}/Libraries",
	}

	defines { }
	
	filter "configurations:Debug"
		
	filter "configurations:Release"

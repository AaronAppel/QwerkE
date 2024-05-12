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
		"%{wks.location}/Source/Libraries",
	}

	defines { }
	
	filter "configurations:Debug"
		
	filter "configurations:Release"

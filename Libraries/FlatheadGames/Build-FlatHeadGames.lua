project "FlatHeadGames"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs	{ }
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "_Q32Bit" }

	filter "configurations:Release"
		defines { "_Q64Bit" }

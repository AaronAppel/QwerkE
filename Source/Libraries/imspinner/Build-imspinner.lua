project "imspinner"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
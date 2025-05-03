project "ImNodeFlow"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp",
		"**.inl",
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
	
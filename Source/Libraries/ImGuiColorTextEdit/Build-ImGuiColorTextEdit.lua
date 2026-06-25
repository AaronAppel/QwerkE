project "ImGuiColorTextEdit"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp",
	}

	removefiles
	{
		"docs/**",
		"example/**",
	}
	
	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
project "imgui-spin-value"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
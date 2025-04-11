project "imgui-console"
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
		"%{wks.location}/Source/Libraries/imgui-console",
	}
	
	links
	{
		"imgui"
	}
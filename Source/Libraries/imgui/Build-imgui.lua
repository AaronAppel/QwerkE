project "imgui"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
		"%{wks.location}/Source/Libraries/glfw",
	}
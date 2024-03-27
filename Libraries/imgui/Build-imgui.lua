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
		"%{wks.location}/Libraries/imgui",
		"%{wks.location}/Libraries/glfw",
	}
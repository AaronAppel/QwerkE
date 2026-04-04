project "imgui-node-editor"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.inl",
		"**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
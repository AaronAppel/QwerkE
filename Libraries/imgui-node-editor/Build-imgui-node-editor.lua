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
		"%{wks.location}/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
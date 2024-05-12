project "imgui-node-editor"
	kind "StaticLib"
	location ""

	files
	{
		"**.h",
		"**.inl",
		"**.cpp"
	}
	
	removefiles { "utilities/**" } -- Experimental files

	includedirs
	{
		"%{wks.location}/Source/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
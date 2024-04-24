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
		"%{wks.location}/Libraries/imgui",
	}
	
	links
	{
		"imgui"
	}
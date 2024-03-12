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
		"%{wks.location}/Libraries/" -- Required by 3rd party libraries
	}

	defines { }
	
	links
	{
		"%{LibraryLib.glfw}",
	}

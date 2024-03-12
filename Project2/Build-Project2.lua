project "Project2"
	kind "ConsoleApp"
	location ""
	
	defines	{ }
	debugargs { }
	postbuildcommands { }

	files { "*.h", "*.cpp", }

	includedirs
	{
		"%{wks.location}/bgfxExampleOrginal",
	}

	links
	{
		"bgfxExampleOrginal",
	}

	filter "system:windows"
	   systemversion "latest"
	   defines { "WINDOWS" }

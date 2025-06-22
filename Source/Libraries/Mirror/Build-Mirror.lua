project "Mirror"
	kind "StaticLib"
	location ""
	
	files
	{ 
		"**.h",
		"**.cpp",
	}
	
	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" } -- bx dependency
		
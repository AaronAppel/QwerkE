include "Libraries.lua"

project "Common"
	kind "StaticLib"
	location ""
	
	files
	{ 
		"**.h",
		"**.cpp",
	}

	includedirs
	{
		"%{wks.location}/", -- Root for #including "Libraries/..."
	}

	defines { LibraryDefines }

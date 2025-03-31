include "Libraries.lua"

project "Common"
	kind "StaticLib"
	location ""
	
	files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", "%{prj.name}/**.hpp", "%{prj.name}/**.c" }

	includedirs
	{
		"%{wks.location}/Source/", -- Root for #including "Libraries/..."
	}

	defines { LibraryDefines }

	forceincludes { "QC_PCH.h", }
	
	pchheader "QC_PCH.h"
	pchsource "%{prj.name}/QC_PCH.cpp"
	
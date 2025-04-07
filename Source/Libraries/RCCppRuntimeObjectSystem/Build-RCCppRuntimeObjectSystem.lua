project "RCCppRuntimeObjectSystem"
	kind "StaticLib"
	location ""
	
	files
	{ 
		"**.h",
		"**.cpp",
	}
	
	links
	{
		"RCCppRuntimeCompiler"
	}
	
	includedirs
	{
		"%{wks.location}/Source/Libraries/RCCppRuntimeCompiler/RuntimeCompiler/",
	}
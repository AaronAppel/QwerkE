project "RCCppQwerkE"
	kind "StaticLib"
	location ""
	
	files
	{ 
		"**.h",
		"StdioLogSystem.cpp",
	}
	
	links
	{
		"RCCppRuntimeCompiler",
		"RCCppRuntimeObjectSystem",
	}
	
	includedirs
	{
		"%{wks.location}/Source/Libraries/RCCppRuntimeCompiler/RuntimeCompiler/",
		"%{wks.location}/Source/Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/",
	}
	
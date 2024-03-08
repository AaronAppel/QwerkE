project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	location ""
	staticruntime "off"

	targetdir ("../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../bin/int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"**.h",
		"**.cpp",
		"**.inl",
		"**.sc",
	}

	includedirs	{ }

	defines { }
	
	links
	{
		"bx",
	}
		
	filter "configurations:*"
		architecture "x86"
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"

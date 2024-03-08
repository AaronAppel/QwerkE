project "imgui"
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
		"**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Libraries/" -- Required by 3rd party libraries
	}

	defines { "_QDEARIMGUI" }
	
	links
	{
		"%{LibraryLib.glfw}",
	}
		
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

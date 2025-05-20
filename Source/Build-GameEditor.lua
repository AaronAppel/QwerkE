project "GameEditor"
	kind "ConsoleApp"
	location ""
	
	defines
	{
		"WorkspaceRootDir=\"%{wks.location}/\"",
		
		-- Command line arguments :
		"key_OverrideAssetsDirPath=\"-overrideAssetsDirPath\"",
		"key_OverrideNullAssetsDirPath=\"-overrideNullAssetsDirPath\"",
		
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_FORMAT_MACROS",
	}
	
	debugargs -- #NOTE User setting changes require VS reload
	{
		"-overrideAssetsDirPath", "\"%{wks.location}Assets\"",
		"-overrideNullAssetsDirPath", "\"%{wks.location}Assets\\NullAssets\"",
	}
	
	postbuildcommands
	{
		-- Starts in %{wks.location}/%{prj.name} (PremakeProject/Game) directory
		"call ../Build/PostBuild.bat \"../Source/Libraries/\" \"../bin/" .. OutputDir .. "/%{prj.name}/\"",
	}

	files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", "%{prj.name}/**.hpp", "%{prj.name}/**.c", }

	includedirs
	{
		"%{wks.location}/Source/Common", -- #TODO Review if needed. Was at the time of adding
		"%{wks.location}/Source/Framework",
		"%{wks.location}/Source", -- #NOTE Root for accessing library source
		"%{wks.location}/Source/Libraries/", -- #NOTE Required by 3rd party libraries
		
		"%{wks.location}/Source/Libraries/bx/include",
		"%{wks.location}/Source/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Source/Libraries/bgfx/3rdparty",
		"%{wks.location}/Source/Libraries/bgfx/include",
		"%{wks.location}/Source/Libraries/bgfxFramework/include",
	}

	links -- Project references
	{
		"Editor",
		"Framework",
	}

	-- Linker Additional Options "/NODEFAULTLIB:msvcrt.lib" to disable warning
	-- ignoredefaultlibraries { "msvcrt" }
	-- Also warning LNK4006: __NULL_IMPORT_DESCRIPTOR already defined in glew32d.lib(glew32d.dll); second definition ignored

	-- filter "configurations:*32"
	-- 	architecture "x86"
	--	defines { "_Q32BIT", }

	-- filter "configurations:*64"
	-- 	architecture "x86_64"
	--	defines { "_Q64BIT", }
	
	filter "system:windows"
	   systemversion "latest"
	   defines { "WINDOWS" }

	filter "configurations:Debug" -- #TODO Remove LibrariesDir
	   defines
		{
			"_QDEBUG",
			"BX_CONFIG_DEBUG=1", -- Required by bgfx
			"LibrariesDir=\"%{wks.location}/Libraries/\"",
			LibraryDefines
		}

	filter "configurations:Release"
	   defines { "_QRELEASE", "BX_CONFIG_DEBUG=0", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }

	filter "configurations:Retail"
	   defines { "_QRETAIL", "BX_CONFIG_DEBUG=0", "LibrariesDir=\"%{wks.location}/Libraries/\"", LibraryDefines }
		runtime "Release"
		symbols "off"
		optimize "on"

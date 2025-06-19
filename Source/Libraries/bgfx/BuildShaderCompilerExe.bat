rem BGFX documentation: https://bkaradzic.github.io/bgfx/build.html
rem Originally from: https://github.com/JimmyLord/bgfxFramework File: Tools/BuildShaderCompiler.bat
@..\bx\tools\bin\windows\genie --with-tools vs2022

@rem Attempted to build from command line, but fails due to windows SDK version.
@rem call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@rem msbuild .build\projects\vs2017\bgfx.sln /property:Configuration=Release -m

@echo ---
@echo Visual studio will be launched, upgrade the project, switch to release mode and build all projects in the tools/shaderc folder
@echo ---
@pause

@start .build\projects\vs2022\bgfx.sln /property:Configuration=Release

@echo ---
@echo Wait for the build to finish, it takes a while
@echo ---
@pause

@echo ---
@echo Copying ".build\win64_vs2022\bin\shadercRelease.exe" to "shaderc.exe"
@echo ---
@copy .build\win64_vs2022\bin\shadercRelease.exe shaderc.exe
@pause

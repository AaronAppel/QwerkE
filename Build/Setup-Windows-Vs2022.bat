@echo off

IF EXIST "Build\" (
	cd Build
)

set PREMAKE_EXE=premake5.exe
set PREMAKE_FILE="premake.lua"
 
%PREMAKE_EXE% --file=%PREMAKE_FILE% vs2022


IF NOT %errorlevel%==0 (
	echo Encountered error!
	pause
) ELSE (
	tasklist /FI "IMAGENAME eq devenv.exe" | find /i "devenv.exe"
	
	rem Only open solution automatically for me
	IF Aaron==%username% (
		IF ERRORLEVEL 1 (
			rem Only open solution if no instances of visual studio are open
			START ../QwerkE.sln
		)
	)
)
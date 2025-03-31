@echo off

premake5.exe --file="premake.lua" vs2022

IF NOT %errorlevel%==0 (
	echo Encountered error!
	pause
)
ELSE
(
	cd ../
	tasklist /FI "IMAGENAME eq devenv.exe" | find /i "devenv.exe"
	
	rem Only open solution automatically for me
	IF Aaron==%username% (
		IF ERRORLEVEL 1 (
			rem Only open solution if no instances of visual studio are open
			START QwerkE.sln
		)
	)
)
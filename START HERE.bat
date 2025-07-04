
start Source\Libraries\bgfx\1stTimeSetup.bat
echo Building bgfx shader and mesh compilers

rem Create shortcut to build script to use from now on
set TARGET='Build\Setup-Windows-Vs2022.bat'
set SHORTCUT='Build Vs 2022 Windows.lnk'
set PWS=powershell.exe -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile

rem #TODO Verify shortcut creation
%PWS% -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut(%SHORTCUT%); $S.TargetPath = %TARGET%; $S.Save()"

rem Build solution files
pushd Build
call premake5.exe --file="Build-QwerkE.lua" vs2022
popd

@echo off

IF EXIST QwerkE.sln (

	echo BUILD SUCCEEDED
	
	rem For fun
	echo.
	ping -n 2 127.0.0.1 >nul
	echo This file will now self destruct in...

	FOR %%A IN (4, 3, 2, 1) DO (
		ping -n 2 127.0.0.1 >nul
		call echo %%A...
	)

	echo.
	ping -n 2 127.0.0.1 >nul
	echo Goodbye World!
	ping -n 3 127.0.0.1 >nul
	@echo on
	
	START QwerkE.sln
	
	rem Ignore changes to this file in git
	git update-index --assume-unchanged "%~f0"
	
	rem Delete this bat file as it's no longer needed
	start /b "" cmd /c del "%~f0"&exit /b
	
) ELSE (
	echo BUILD FAILED
)
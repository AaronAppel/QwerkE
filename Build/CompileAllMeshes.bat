rem BGFX documentation: https://bkaradzic.github.io/bgfx/build.html

rem From: https://stackoverflow.com/questions/17063947/get-current-batchfile-directory
rem Change to directory of this file so call executes the same from any caller
rem echo %~dp0
cd %~dp0

rem For !errorlevel! to be evaluated on execution
@setlocal EnableDelayedExpansion

@echo off

set meshCompiler=..\Source\Libraries\bgfx\.build\win64_vs2022\bin\geometrycRelease.exe
set outputMeshesFolder=..\Assets\Meshes\Bin
set sourceMeshesFolder=..\Assets\Meshes

@mkdir %outputMeshesFolder%

@call; rem Clear the %errorlevel%

rem .obj files
@FOR %%G in ("%sourceMeshesFolder%\*.obj") DO (
	rem #TODO Errors are being detected incorrectly. Could be incorrectly expanding !errorlevel!. Could Check casing
	echo Compiling %%~nG
	%meshCompiler% -f %%G -o %outputMeshesFolder%\%%~nG.bin
	rem echo %%~nG file name no extension
	rem echo %%~nxG file name
	rem echo %%~dpnG absolute file path no extension
	rem echo %%~G relative file path
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built dx11 %%G)
)

IF !errorlevel! NEQ 0 (goto error)

goto :end

:error
echo.
pause
exit /B 1

:end
echo Successfully compiled meshes
rem pause

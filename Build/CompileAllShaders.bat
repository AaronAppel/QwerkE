rem BGFX documentation: https://bkaradzic.github.io/bgfx/build.html

rem From: https://stackoverflow.com/questions/17063947/get-current-batchfile-directory
rem Change to directory of this file so call executes the same from any caller
rem echo %~dp0
cd %~dp0

rem For !errorlevel! to be evaluated on execution
@setlocal EnableDelayedExpansion

@echo off

set shaderCompiler=..\Source\Libraries\bgfx\.build\win64_vs2022\bin\shadercRelease.exe
set outputShadersFolder=..\Assets\Shaders\Bin
set sourceShadersFolder=..\Assets\Shaders
set bgfxSource=..\Source\Libraries\bgfx\src

rem Additional targets: pssl, nvm, metal
@mkdir %outputShadersFolder%
@mkdir %outputShadersFolder%\dx11
@mkdir %outputShadersFolder%\glsl
@mkdir %outputShadersFolder%\spirv

@call; rem Clear the %errorlevel%

rem Vertex files
@FOR %%G in ("%sourceShadersFolder%\*.vert") DO (
	rem #TODO Errors are being detected incorrectly. Could be incorrectly expanding !errorlevel!. Could Check casing
	echo Compiling %%~nG
    %shaderCompiler% -f %%G -o %outputShadersFolder%\dx11\%%~nxG.bin  --type v --platform windows -i %bgfxSource% -p s_5_0
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\glsl\%%~nxG.bin  --type v --platform windows -i %bgfxSource% -p 440
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built glsl %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\spirv\%%~nxG.bin --type v --platform windows -i %bgfxSource% -p spirv
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built spirv %%G)
)

IF !errorlevel! NEQ 0 (goto error)

rem Fragment files
@FOR %%G in ("%sourceShadersFolder%\*.frag") DO (
	rem #TODO Errors are being detected incorrectly. Could be incorrectly expanding !errorlevel!. Could Check casing	
	echo Compiling %%~nG
    %shaderCompiler% -f %%G -o %outputShadersFolder%\dx11\%%~nxG.bin  --type f --platform windows -i %bgfxSource% -p s_5_0
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\glsl\%%~nxG.bin  --type f --platform windows -i %bgfxSource% -p 440
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built glsl %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\spirv\%%~nxG.bin --type f --platform windows -i %bgfxSource% -p spirv
    rem IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built spirv %%G)
)

goto :end

:error
echo.
pause
exit /B 1

:end
echo Successfully compiled shaders
pause

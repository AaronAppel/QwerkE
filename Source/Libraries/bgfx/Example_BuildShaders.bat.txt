rem BGFX documentation: https://bkaradzic.github.io/bgfx/build.html

echo Example file only
pause
exit 0

@setlocal EnableDelayedExpansion rem For !errorlevel! to be evaluated on execution
@echo off

rem Path to source .vert and .frag shaders
set sourceShadersFolder=..\Assets\Shaders
rem Path to shaderc.exe
set shaderCompiler=shaderc.exe
rem Path to save output shader binary files
set outputShadersFolder=..\Assets\Shaders\Bin
rem Path to bgfx src code folder
set bgfxSource=..\Source\Libraries\bgfx\src

rem Create output directories ahead of compilation
rem Additional targets: pssl, nvm, metal
@mkdir %outputShadersFolder%
@mkdir %outputShadersFolder%\dx11
@mkdir %outputShadersFolder%\glsl
@mkdir %outputShadersFolder%\spirv

@call; rem Clear the %errorlevel%

rem Vertex files
@FOR %%G in ("%sourceShadersFolder%\*.vert") DO (
    %shaderCompiler% -f %%G -o %outputShadersFolder%\dx11\%%~nxG.bin  --type v --platform windows -i %bgfxSource% -p s_5_0
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\glsl\%%~nxG.bin  --type v --platform windows -i %bgfxSource% -p 440
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built glsl %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\spirv\%%~nxG.bin --type v --platform windows -i %bgfxSource% -p spirv
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built spirv %%G)
)

IF !errorlevel! NEQ 0 (goto error)

rem Fragment files
@FOR %%G in ("%sourceShadersFolder%\*.frag") DO (
    %shaderCompiler% -f %%G -o %outputShadersFolder%\dx11\%%~nxG.bin  --type f --platform windows -i %bgfxSource% -p s_5_0
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\glsl\%%~nxG.bin  --type f --platform windows -i %bgfxSource% -p 440
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built glsl %%G)
    %shaderCompiler% -f %%G -o %outputShadersFolder%\spirv\%%~nxG.bin --type f --platform windows -i %bgfxSource% -p spirv
    IF !errorlevel! NEQ 0 (echo Error(s) in %%G && goto error) ELSE (echo Built spirv %%G)
)

goto :end

:error
echo.
pause
exit /B 1

:end
Success
rem pause

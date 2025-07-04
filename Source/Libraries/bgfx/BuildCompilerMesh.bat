call BuildBgfxSolutionTools.bat

set msbuildPath="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set vcxProjectPath=".build\projects\vs2022\geometryc.vcxproj"

rem msbuild MySolution.sln /t:MyProjectName /p:Configuration=Debug /p:Platform="Any CPU"
rem msbuild MyProject\MyProject.vcxproj /p:Configuration=Release
%msbuildPath% %vcxProjectPath% /p:Configuration=Release /p:Platform="x64"

IF !errorlevel! NEQ 0 (goto error)

goto :end

:error
echo May have encountered error(s)
pause
exit /B 1

:end
echo Successfully built geometryc.exe
rem pause

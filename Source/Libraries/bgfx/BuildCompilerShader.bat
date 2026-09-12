call BuildSolutionTools.bat

set msbuildPath="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

if not exist %msbuildPath%
echo Could not find VS 2022 Community install in expected path: %msbuildPath%
exit /B 1

set vcxProjectPath=".build\projects\vs2022\shaderc.vcxproj"

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

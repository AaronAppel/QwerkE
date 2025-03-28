rem XCOPY documentation
rem https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/xcopy
rem https://ss64.com/nt/xcopy.html

rem xcopy s:\ z:\* /S /Y /D
rem /S will copy all subfolders and files within them Y prevents any overwrite prompts D essentially copies anything newer than what is already in the destination

rem Microsoft Documentation
rem Copies directories and subdirectories, unless they're empty. If you omit /s, xcopy works within a single directory.
rem /Y Suppresses prompting to confirm that you want to overwrite an existing destination file.
rem /D Copies source files changed on or after the specified date only. If you don't include a MM-DD-YYYY value, xcopy copies all source files that are newer than existing destination files. This command-line option allows you to update files that have changed.
rem '*' at the end of the destination to surpress directory/file prompt

@echo on
echo in CopyLibraryDLLs.bat
echo Called from directory: %~dp0
echo Working dir: %cd%

@echo off
set sourceDir="..\Libraries\"
IF NOT "%~1"=="" set sourceDir=%~1

set targetDir="..\bin\windows-x86-Debug\Game\"
IF NOT "%~2"=="" set targetDir=%~2

echo Source dir: %sourceDir%
echo Target dir: %targetDir%

rem @echo off
rem xcopy "%sourceDir%assimp\Release\x86\assimp-vc140-mt.dll" "%targetDir%assimp-vc140-mt.dll*" /Y /D
rem xcopy "%sourceDir%freetype2\x86\freetype.dll" "%targetDir%freetype.dll*" /Y /D
rem xcopy "%sourceDir%glfw\glfw3.dll" "%targetDir%glfw3.dll*" /Y /D
rem xcopy "%sourceDir%glew\glew32d.dll" "%targetDir%glew32d.dll*" /Y /D
rem xcopy "%sourceDir%OpenAL\Win32\OpenAL32.dll" "%targetDir%OpenAL32.dll*" /Y /D
rem xcopy "%sourceDir%OpenAL\soft_oal.dll" "%targetDir%soft_oal.dll*" /Y /D
rem xcopy "%sourceDir%pThreads\lib\x86\pthreadVC2.dll" "%targetDir%pthreadVC2.dll*" /Y /D
rem xcopy "%sourceDir%RakNet\RakNet_VS2008_DLL_Debug_Win32.dll" "%targetDir%RakNet_VS2008_DLL_Debug_Win32.dll*" /Y /D
rem xcopy "%sourceDir%RakNet\RakNet_VS2008_DLL_Release_Win32.dll" "%targetDir%RakNet_VS2008_DLL_Release_Win32.dll*" /Y /D

exit %errorlevel%
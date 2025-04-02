
pushd Build
call premake5.exe --file="premake.lua" vs2022
popd

IF EXIST QwerkE.sln (
	START QwerkE.sln

	set TARGET='Build\Setup-Windows-Vs2022.bat'
	set SHORTCUT='Build Project Windows Vs 2022.lnk'
	set PWS=powershell.exe -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile

	%PWS% -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut(%SHORTCUT%); $S.TargetPath = %TARGET%; $S.Save()"

	del START_HERE.bat
)
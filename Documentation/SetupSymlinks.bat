set readmeDir=
IF EXIST "%readmeDir%README.md" (
	rem Just to ensure the target file is in fact a symlink
	del "%readmeDir%README.md"
)
mklink /H "%readmeDir%README.md" ..\README.md

set assetsDir=Z. Obsidian\Documentation Assets\

mklink /H "%assetsDir%Symlinked_core-plugins.md" .obsidian\core-plugins.json
mklink /H "%assetsDir%Symlinked_community-plugins.md" .obsidian\community-plugins.json
set readmeDir=
mklink /H "%readmeDir%README.md" ..\README.md

set assetsDir=Z. Obsidian\Documentation Assets\

rem Another type of link might let the copy be read only
mklink /H "%assetsDir%Symlinked_core-plugins.md" .obsidian\core-plugins.json

rem Another type of link might let the copy be read only
mklink /H "%assetsDir%Symlinked_community-plugins.md" .obsidian\community-plugins.json
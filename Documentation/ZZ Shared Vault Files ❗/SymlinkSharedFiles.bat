rem For sym-linking shared files between multiple Obsidian vaults
rem using a local google drive folder for backups

set newFile="SourceFile.ext"
set sourceFile="..."
rem mklink /H %newFile% %sourceFile%

set newDir="Shared Vault Files ❗"
set sourceDir="F:\Google Drive\Shared Vault Files ❗"
mklink /J %newDir% %sourceDir%

rem NOT WORKING WITH SPECIAL❗
rem Copy/pasting the command below into a command terminal should work.
rem mklink /J "Shared Vault Files ❗" "F:\Google Drive\Shared Vault Files ❗"

rem *Remember that the folder cannot be moved in any way or it moves the contents
rem of the folder for every linked view and shortcut. Delete and recreate the symlink
rem in the new desired location instead.
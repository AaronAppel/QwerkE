## New Git Repo
New .gitignore file contents example :
![[template.gitignore]]

## New Obsidian Vault
Check Google Drive folder for latest symlink .bat file.
This is used to link common shared files between Obsidian vaults.
Last folder dir : F:\Google Drive\Shared Vault Files ❗
Last symlink command :
```bash title:SymlinkSharedFiles.bat
mklink /J "Shared Vault Files ❗" "F:\Google Drive\Shared Vault Files ❗"
```

New Visual Studio Project
Cleanup.bat file contents :
! [[Cleanup.bat]]

Last Cleanup.bat commands :
```bash title:Cleanup.bat
rem cd ../  
  
rem Delete all .dll files in engine root directory  
del *.dll  
  
del /Q /F instrumentor_log.json  
  
del /Q /F /S /AH .vs  
del /Q /F /S x64  
rmdir /Q /S ".vs"  
rmdir /Q /S x64  
  
del /Q /F /S /AH Debug  
rmdir /Q /S Debug  
  
del /Q /F /S /AH Release  
rmdir /Q /S Release  
  
cd QwerkE_Framework  
call Cleanup.bat  
cd ../
```

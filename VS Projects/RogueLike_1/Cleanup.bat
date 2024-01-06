cd RogueLike_1
del /Q /F /S /AH .vs\*
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S .vs
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release

cd RogueLike_Win32
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release

cd ../
cd ../

cd QwerkE_Framework
call Cleanup.bat
cd ../
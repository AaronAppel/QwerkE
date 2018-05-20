del /Q /F /S /AH .vs\*
del /Q /F /S x64
del /Q /F /S enc_temp_folder
rmdir /Q /S .vs
rmdir /Q /S x64

del /Q /F /S /AH Debug
rmdir /Q /S Debug

del /Q /F /S /AH Release
rmdir /Q /S Release

cd QwerkE_Framework
call Cleanup.bat
cd ../

cd QwerkE_Documentation
call Cleanup.bat
cd ../

cd Test_Game1
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
cd ../

cd QwerkE_Win32
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
cd ../

cd Test_GameClient
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
cd ../

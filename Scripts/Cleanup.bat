cd ../

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

cd Development

cd RenderingEnv
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
rmdir /Q /S ".vs"
cd ../

cd Games

cd Test_Game1
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
rmdir /Q /S ".vs"
cd ../

cd Test_GameClient
del /Q /F /S Debug
del /Q /F /S x64
del /Q /F /S Release
rmdir /Q /S Debug
rmdir /Q /S x64
rmdir /Q /S Release
rmdir /Q /S ".vs"
cd ../

cd QwerkE_Framework
call Cleanup.bat
cd ../
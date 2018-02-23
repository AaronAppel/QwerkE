del /Q /F /S /AH .vs\*
del /Q /F /S x64
rmdir /Q /S .vs
rmdir /Q /S x64

del /Q /F /S /AH Debug
rmdir /Q /S Debug

cd Test_Game1
del /Q /F /S Debug
del /Q /F /S x64
rmdir /Q /S Debug
rmdir /Q /S x64
cd ../

cd QwerkE_Win32
del /Q /F /S Debug
del /Q /F /S x64
rmdir /Q /S Debug
rmdir /Q /S x64
cd ../

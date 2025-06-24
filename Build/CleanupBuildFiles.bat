
del shaderc.exe

rem Start in repo root dir
cd ../

del /Q /F /S /AH enc_temp_folder
rmdir /Q /S "enc_temp_folder"

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

del /Q /F /S /AH bin
rmdir /Q /S bin

del *.sln

if exist Assets\Shaders\Bin\ (
	del /Q /F /S /AH Assets\Shaders\Bin
	rmdir /Q /S Assets\Shaders\Bin
)

pushd Source
call Cleanup.bat
popd

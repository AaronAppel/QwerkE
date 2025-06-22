del shaderc.exe

rem Start in repo root dir
cd ../

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

del /Q /F /S /AH bin
rmdir /Q /S bin

del *.sln

pushd Source
call Cleanup.bat
popd

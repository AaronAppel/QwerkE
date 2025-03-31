rem pushd ..
cd ../
rem del *.dll
rem del *.vcxproj

del /Q /F /S /AH .vs
del *.sln

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

pushd Source
call Cleanup.bat
popd

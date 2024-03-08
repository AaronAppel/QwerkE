rem Delete all .dll files in engine root directory
cd ../
del *.dll
del *.sln
del *.vcxproj

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

pushd Framework/
call Cleanup.bat
popd

pushd Editor/
call Cleanup.bat
popd

pushd Game/
call Cleanup.bat
popd
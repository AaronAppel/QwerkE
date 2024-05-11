rem pushd ..
cd ../
rem del *.dll
rem del *.vcxproj

del *.sln

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

pushd Framework
call Cleanup.bat
popd

pushd Game
call Cleanup.bat
popd

pushd Libraries
call Cleanup.bat
popd

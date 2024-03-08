cd ../
del *.dll
del *.sln
del *.vcxproj

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

cd Framework/
call Cleanup.bat
cd ../

cd Editor/
call Cleanup.bat
cd ../

cd Game/
call Cleanup.bat
cd ../

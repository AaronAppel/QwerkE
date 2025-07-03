IF NOT EXIST .build (
	cd ../
)

IF EXIST .build (
	del /Q /F /S /AH .build
	rmdir /Q /S ".build"
)
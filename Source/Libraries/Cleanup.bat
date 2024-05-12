
rem #TODO Loop over every directory instead of manually entering libraries

cd bgfx
del *.vcxproj
del *.user
cd ../

pushd bgfxFramework
del *.vcxproj
del *.user
popd

pushd bimg
del *.vcxproj
del *.user
popd

pushd bx
del *.vcxproj
del *.user
popd

pushd cJSON
del *.vcxproj
del *.user
popd

pushd FlatHeadGames
del *.vcxproj
del *.user
popd

pushd Mirror
del *.vcxproj
del *.user
popd

pushd spdlog
del *.vcxproj
del *.user
popd

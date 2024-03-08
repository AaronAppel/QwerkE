
LibraryDefines = {
-- _QALLCAPSNODELIMETERS
-- "_QASSIMP",
"_QCJSON",
"_QDEARIMGUI",
"_QENTT",
"_QENUM",
"_QFLATHEADGAMES",
-- "_QFREETYPE2",
"_QGLEW",
"_QGLFW3",
"_QGLM",
"_QLODEPNG",
"_QMIRROR",
"_QOPENGL",
"_QOPENAL",
"_QPTHREADS",
"_QSPDLOG",
"_QSTBIMAGE",
}

LibraryLib = {}
-- LibraryLib["assimp"] = "%{wks.location}/Libraries/assimp/Release/x86/assimp-vc140-mt.lib"
-- LibraryLib["freetype2"] = "%{wks.location}/Libraries/freetype2/x86/freetype.lib"
LibraryLib["glew"] = "%{wks.location}/Libraries/glew/glew32d.lib"
LibraryLib["glfw"] = "%{wks.location}/Libraries/glfw/glfw3.lib"
LibraryLib["OpenAL"] = "%{wks.location}/Libraries/OpenAL/Win32/OpenAL32.lib"
LibraryLib["pthreads"] = "%{wks.location}/Libraries/pThreads/lib/x86/pthreadVC2.lib"

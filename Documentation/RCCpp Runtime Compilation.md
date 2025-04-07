
https://github.com/RuntimeCompiledCPlusPlus
or working GLFW + dear imgui example:
https://github.com/enkisoftware/RCCpp-DearImGui-GLFW-example
https://runtimecompiledcplusplus.blogspot.com/

Disabling in release builds :
https://github.com/RuntimeCompiledCPlusPlus/RuntimeCompiledCPlusPlus/wiki/Disabling-runtime-compilation

Runtime compilation can be disabled in one of three ways:
1. By setting the RCCPPOFF macro definition in your project.
2. By calling RuntimeObjectSystem::SetAutoCompile( false ).
3. By not updating the file change notifier.
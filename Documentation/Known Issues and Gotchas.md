## OpenAL    [#Libs](obsidian://open?vault=Documentation&file=References%2FLibrary%20References)
If soft_oal.dll is not copied to the correct .dll directory with other .dlls like OpenAL32.dll, the calls to al and alc code will return null. You may get useful info from alGetError(), but it can be hard to determine the issue. Make sure the soft_oal.dll is copied correctly to avoid future headaches.

## Visual Studio Symbol Server
An issue relating to Visual Studio symbol server issues can be solved by killing the symbol server with this command :
TASKKILL /F /IM mspdbsrv.exe

## Bullet Library files
When moving bullet files, it's important to remember to NOT add any .cpp files to the solution. The \#includes won't work as QwerkE is currently configured.

## GLM Math Library
Same issue as Bullet with dummy.cpp, where it shouldn't be added to the Visual Studio solution or project.
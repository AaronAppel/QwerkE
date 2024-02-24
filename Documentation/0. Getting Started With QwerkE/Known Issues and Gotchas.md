## OpenAL                        [[Library References]]
If soft_oal.dll is not copied to the correct .dll directory with other .dlls like OpenAL32.dll, the calls to al and alc code will return null. You may get useful info from alGetError(), but it can be hard to determine the issue. Make sure the soft_oal.dll is copied correctly to avoid future headaches.

## Visual Studio Symbol Server
An issue relating to Visual Studio symbol server issues can be solved by killing the symbol server with this command :
TASKKILL /F /IM mspdbsrv.exe

## Bullet Library files       [[Library References]]
When moving bullet files, it's important to remember to NOT add any .cpp files to the solution. The \#includes won't work as QwerkE is currently configured.

## GLM Math Library      [[Library References]]
Same issue as Bullet with dummy.cpp, where it shouldn't be added to the Visual Studio solution or project.

## ImGui                   [[Library References]]
Using the docking space feature, a crash will occur if two windows are ever docked to 1 area/section/window. Probably an issue on my end, but all libraries are using old versions so new versions could include fixes for issues like this.
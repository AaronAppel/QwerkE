Change console window visibility: 
```
#include <Windows.h>
::ShowWindow(::GetConsoleWindow(), ::IsWindowVisible(::GetConsoleWindow()) ? SW_HIDE : SW_SHOW);
```

The windows console window cannot be disabled through data or code changes. Must be modified in VS project.
Change:
Configuration Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup
and
Configuration Properties -> Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
But then the window cannot be opened at runtime as the code has been ignored from compilation.
Do this if you want to have an in window or custom console.

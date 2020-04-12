#ifndef _Game1_H_
#define _Game1_H_

// #include <sdkddkver.h>
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// #include "../../../Source/Headers/Engine_Defines.h"

#if _WIN32
#define Win32Bit
#else
#define Win64Bit
#endif

#define FrameworkDir "../../QwerkE_Framework/"
#define AssetsDir "../../Assets/"
// #define LibrariesDir "../../QwerkE_Framework/Libraries/"

#if 1 // Use the Engine?
#include "../../../Source/Engine.h"
#else // Or just the Framework?
#include "../../../QwerkE_Framework/QwerkE.h"
#endif

#endif // _Game1_H_

#ifndef _Main_H_
#define _Main_H_

#define Win32Bit
#define FrameworkDir "../../QwerkE_Framework/"
#define AssetsDir "Assets/"
// #define LibrariesDir "../../QwerkE_Framework/QwerkE_Common/Libraries/"

#if 1 // Use the Engine?
#include "../Qwerk_Engine/Shared_Engine/Engine.h"
#else // Or just the Framework?
#include "../../QwerkE_Framework/QwerkE.h"
#endif

#endif // !_Main_H_
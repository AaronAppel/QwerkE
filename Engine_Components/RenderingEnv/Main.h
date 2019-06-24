#ifndef _Main_H_
#define _Main_H_

#define DEBUG 1
#define x86 1

#if x86
#define Win32Bit
#else
#define Win64Bit
#endif // x86

#define FrameworkDir "../../QwerkE_Framework/" // TODO: Not being used
#define AssetsDir "Assets/" // TODO: Not being used
// #define LibrariesDir "../QwerkE_Common/Libraries/"
// #include "../../QwerkE_Framework/QwerkE.h"
#include "../Qwerk_Engine/Shared_Engine/Engine.h"

#endif // !_Main_H_
#ifndef _QwerkE_Windows_H_
#define _QwerkE_Windows_H_

#define DEBUG 1
#define Isx86 1

#if Isx86
#define Win32Bit
#else
#define Win64Bit
#endif // x86

 #ifdef _WIN64
 #define Win64Bit
 #elif defined(_WIN32)
 #define Win32Bit
 #else
 #pragma warning "Define platform!"
 #endif

#define FrameworkDir "QwerkE_Framework/"
#define AssetsDir "Assets/"
#include "../Engine.h"
// #include "../../QwerkE_Framework/QwerkE.h"

#endif // !_QwerkE_Windows_H_

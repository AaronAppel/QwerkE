#ifndef _QwerkE_Windows_H_
#define _QwerkE_Windows_H_

#define _QWindows

 #ifdef _WIN64
 #define Win64Bit
 #elif defined(_WIN32)
 #define Win32Bit
 #else
 #pragma warning "Define platform!"
 #endif

#include "../Engine.h"

#endif // !_QwerkE_Windows_H_

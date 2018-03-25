#ifndef _Engine_Platform_H_
#define _Engine_Platform_H_

// platform includes
#if defined(_Q32bit) && defined(_QWindows)
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(_Q64bit) && defined(_QWindows)
#else
#pragma warning "Define Platform!"
#endif // !platform

#endif // !_Engine_Platform_H_

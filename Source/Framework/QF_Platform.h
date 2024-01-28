#pragma once

// MSVC pre-preocessor defines : https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170

#if _WIN32 || _WIN64
#define _QWindows
#endif

#ifdef _WIN64
#define _Q64Bit
#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <Windows.h>

// #TODO Review includes for 64 bit
// #pragma warning "Support _WIN64!"

#elif (_WIN32)
#define _Q32Bit
#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <Windows.h>
#else
#pragma warning "Define Platform!"
#endif

#if !defined(_Q64Bit) && !defined(_Q32Bit)
#pragma warning "Define target architecture!"
#endif

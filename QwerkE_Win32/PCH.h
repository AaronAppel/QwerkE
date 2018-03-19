#ifndef _PCH_H_
#define _PCH_H_

/////////////////////////////////////////////////
// include STATIC code that does not change often
/////////////////////////////////////////////////

#pragma warning(disable : 4996) // old sprintf warnings in RakNet
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// platform includes
#if _WIN32
// include inside WIN32 projects to use QwerkE framework
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WIN32

// standard libraries
// map, vector, string, etc
#include <string>

// libraries
#include "../Shared_Generic/Libraries_Include.h"

// containers

// math
// #include "Math_Includes.h"

// graphics
// #include "Graphics_Header.h"


#endif // !_PCH_H_

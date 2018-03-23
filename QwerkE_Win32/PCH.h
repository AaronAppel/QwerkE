#ifndef _PCH_H_
#define _PCH_H_

/////////////////////////////////////////////////
// include STATIC code that does not change often
/////////////////////////////////////////////////

#pragma warning(disable : 4996) // old sprintf warnings in RakNet
#define _CRT_SECURE_NO_WARNINGS // old sprintf warnings in RakNet
#define _CRT_SECURE_NO_DEPRECATE // old sprintf warnings in RakNet
#define _WINSOCK_DEPRECATED_NO_WARNINGS // old warnings in RakNet

// platform includes
#if _WIN32
// include inside WIN32 projects to use QwerkE framework
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
//#include <WinSock2.h> // TODO: Remove?
#include <Windows.h>
#endif // _WIN32

// standard libraries
// map, vector, string, etc
// TODO: Is including this here a good idea?
#include <string>
#include <stdio.h>

// constants, enums
#include "Global_Constants.h"
#include "Engine_Enums.h"

// load .lib + .dlls
#include "Load_Libs.h"

// libraries
#include "../Shared_Generic/Libraries_Include.h"

// containers

// math
// #include "Math_Includes.h"

// graphics
// #include "Graphics_Header.h"


#endif // !_PCH_H_

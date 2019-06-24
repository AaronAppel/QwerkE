#ifndef _PCH_Win32_H_
#define _PCH_Win32_H_

/////////////////////////////////////////////////
// PCH_Win32.h
// For faster compiling.
/////////////////////////////////////////////////

#include "QwerkE_Windows.h"

#include "../QwerkE_Framework/Headers/QwerkE_Defines.h"
#include "../QwerkE_Framework/Headers/QwerkE_Platform.h"

// TODO: Move pragma warnings and use pop warnings
//#pragma warning(disable : 4996) // old sprintf warnings in RakNet
//#define _CRT_SECURE_NO_WARNINGS // old sprintf warnings in RakNet
//#define _CRT_SECURE_NO_DEPRECATE // old sprintf warnings in RakNet
//#define _WINSOCK_DEPRECATED_NO_WARNINGS // old warnings in RakNet

// standard libraries
// map, vector, string, etc
// TODO: Is including this here a good idea?
// Does it save compile time?
#include <string>
#include <stdio.h>
#include <map>
#include <vector>

// constants, enums
#include "../QwerkE_Framework/Headers/QwerkE_Global_Constants.h"
#include "../QwerkE_Framework/Headers/QwerkE_Enums.h"

// containers

// libraries
#include "../QwerkE_Framework/Headers/Libraries_Include.h"

/////////////////////////////////////////////////
// include ENGINE code temporarily for faster build times
/////////////////////////////////////////////////
#include "Engine_Additional_Includes.h"

#endif // !_PCH_Win32_H_

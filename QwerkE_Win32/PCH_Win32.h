#ifndef _PCH_Win32_H_
#define _PCH_Win32_H_

/////////////////////////////////////////////////
// include STATIC code that does not change often
/////////////////////////////////////////////////

#include "../Shared_Engine/Engine_Defines.h"
#include "../Shared_Engine/Engine_Platform.h"

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
#include "../Shared_Engine/Global_Constants.h"
#include "../Shared_Engine/Engine_Enums.h"

// containers

// libraries
#include "../Shared_Generic/Libraries_Include.h"

/////////////////////////////////////////////////
// include ENGINE code temporarily for faster build times
/////////////////////////////////////////////////
#include "Additional_Includes.h"

#endif // !_PCH_Win32_H_

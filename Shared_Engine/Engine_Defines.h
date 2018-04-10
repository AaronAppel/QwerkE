#ifndef _Engine_Defines_H_
#define _Engine_Defines_H_

#define IncludeDir "../Shared_Generic/Utilities/FileIO/FileUtilities.h"
// #include IncludeDir

/* Defines values to be used in other ares of code.
   Creates typedefs to make development more convenient*/

// TODO: Define release and debug versions
#ifdef _RELEASE
#define _QRelease
#define NDEBUG // leave out assert()s
#else
#define _QDEBUG
#endif

// TODO: Define API level for various functionality
#define _QTest // Working example to show functionality
#define _QEasy // Beginner API
#define _QMedium // Simple control
#define _QHard // Mixed control
#define _QUltra // Full control

// define platform and architecture
// Compilers: Microsoft C++, GCC, Intel CC
// TODO: Look at how to determine platform and architecture
// TODO: Look at a good way to handle DEBUG vs RELEASE modes
// TOTO: Fix ugly _Q defines
#if _WIN32
#define _QWindows
#define _Q32bit

#elif _WIN64
#define _QWindows
#define _Q64bit

#elif _Android
#elif _Mac
#elif _Linux

#endif // _WIN32

// type definitions
// TODO: Is there an easy way of reducing typing without coupling files like typedef does
typedef unsigned int uint; // less typing and easier to read

#endif // !_Engine_Defines_H_

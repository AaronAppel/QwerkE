#ifndef _Engine_Defines_H_
#define _Engine_Defines_H_

/* Defines values to be used in other ares of code.
   Creates typedefs to make development more convenient*/

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
typedef unsigned int uint; // less typing and easier to read

#endif // !_Engine_Defines_H_

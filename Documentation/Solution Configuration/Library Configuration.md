## Additional Include Path Dependency Injection
Many libraries require modifying a project's [[Additional Includes]] list to point to either the "Libraries/" folder, or further down into the library specific folder. This is inconvenient but should be easily manageable using a build system. Ideally only the "Libraries" folder exists in the [[Additional Includes]] section so QwerkE files can reference library source files with nicer paths.

## Extending Libraries
When looking to extend a libraries source code, instead of manipulating the source files directly which is [[Bad Practice]]

# Loading libraries
See this code file for library loading :
```cpp live:true file:"QF_LibrariesLoad.h" mod:1706466271881 
#pragma once

#include "QF_Defines.h"

//////////////   32 bit Debug    //////////////
#if defined(_Q32Bit) && defined(_QDebug)

#ifdef OpenGL // #TODO Verify debug lib
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma error "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" ) // #TODO Acquire proper lib

#pragma comment( lib, LibrariesDir "freetype2/x86/freetype.lib" ) // #TODO Acquire proper lib

#ifdef GLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Verify proper lib
#endif

#ifdef GLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib
#endif

#ifdef OpenAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Verify proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Acquire proper lib

#ifdef RAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" )
#else
#pragma error "Define networking library!"
#endif

#ifdef BULLET3
#pragma warning( disable : 4099 )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletCollision.lib" )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletDynamics.lib" )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/LinearMath.lib" )
#pragma warning( default : 4099 )
#else
#pragma error "Define physics library!"
#endif

#endif
//-----------   32 bit Debug    -----------//

//////////////   32 bit Release  //////////////
#if defined(_Q32Bit) && defined(_QRelease)

#ifdef OpenGL // #TODO Verify release lib
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" )

#pragma comment( lib, LibrariesDir "freetype2/x86/Release/freetype271MT.lib" )

#ifdef GLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Verify proper lib
#endif

#ifdef GLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3.lib" ) // #TODO Verify proper lib
#endif

#ifdef OpenAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Verify proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Verify proper lib

#ifdef RAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Release_Win32.lib" )
#else
#pragma error "Define networking library!"
#endif

#ifdef BULLET3
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Release/BulletCollision.lib" )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Release/BulletDynamics.lib" )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Release/LinearMath.lib" )
#else
#pragma error "Define physics library!"
#endif

#endif
//-----------   32 bit Release  -----------//

//////////////   64 bit Debug    //////////////
#if defined(_Q64Bit) && defined(_QDebug)

#ifdef OpenGL // #TODO Verify debug lib
// #TODO 64 bit not found on local PC. Need it installed
// #pragma comment( lib, "opengl64.lib" ) // #TODO Found by Visual Studio, but explore better solution
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" ) // #TODO Acquire proper lib

#pragma comment( lib, LibrariesDir "freetype2/x86/freetype.lib" ) // #TODO Acquire proper lib

#ifdef GLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Acquire proper lib
#endif

#ifdef GLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib
#endif

#ifdef OpenAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Acquire proper lib

#ifdef RAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define networking library!"
#endif

#ifdef BULLET3
#pragma warning( disable : 4099 )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletCollision.lib" ) // #TODO Acquire proper lib
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletDynamics.lib" ) // #TODO Acquire proper lib
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/LinearMath.lib" ) // #TODO Acquire proper lib
#pragma warning( default : 4099 )
#else
#pragma error "Define physics library!"
#endif

#endif
//-----------   64 bit Debug    -----------//

//////////////   64 bit Release  //////////////
#if defined(_Q64Bit) && defined(_QRelease)

#ifdef OpenGL // #TODO Verify release lib
// #TODO 64 bit not found on local PC. Need it installed
// #pragma comment( lib, "opengl64.lib" ) // #TODO Found by Visual Studio, but explore better solution
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" ) // #TODO Acquire proper lib

#pragma comment( lib, LibrariesDir "freetype2/x86/freetype.lib" ) // #TODO Acquire proper lib

#ifdef GLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Acquire proper lib
#endif

#ifdef GLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib
#endif

#ifdef OpenAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Acquire proper lib

#ifdef RAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define networking library!"
#endif

#ifdef BULLET3
#pragma warning( disable : 4099 )
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletCollision.lib" ) // #TODO Acquire proper lib
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/BulletDynamics.lib" ) // #TODO Acquire proper lib
#pragma comment( lib, LibrariesDir "Bullet3/lib/Win32/Debug/LinearMath.lib" ) // #TODO Acquire proper lib
#pragma warning( default : 4099 )
#else
#pragma error "Define physics library!"
#endif

#endif
//-----------   64 bit Release  -----------//

```
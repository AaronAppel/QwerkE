#pragma once

//////////////   32 bit Debug    //////////////
#if defined(_Q32Bit) && defined(_QDebug)

#ifdef _QOPENGL // #TODO Verify debug lib
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma error "Define graphics library!"
#endif

#ifdef _QGLEW
// #pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Verify proper lib
#endif

#ifdef _QGLFW3
// #pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib#else
#else
#pragma error "Define window library!"
#endif

#endif
//-----------   32 bit Debug    -----------//

//////////////   32 bit Release  //////////////
#if defined(_Q32Bit) && defined(_QRelease)

#ifdef _QOPENGL // #TODO Verify release lib
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" )

#pragma comment( lib, LibrariesDir "freetype2/x86/Release/freetype271MT.lib" )

#ifdef _QGLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Verify proper lib
#endif

#ifdef _QGLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3.lib" ) // #TODO Verify proper lib
#endif

#ifdef _QOPENAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Verify proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Verify proper lib

#ifdef _QRAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Release_Win32.lib" )
#else
#pragma error "Define networking library!"
#endif

#ifdef _QBULLET3
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

#ifdef _QOPENGL // #TODO Verify debug lib
// #TODO 64 bit not found on local PC. Need it installed
// #pragma comment( lib, "opengl64.lib" ) // #TODO Found by Visual Studio, but explore better solution
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" ) // #TODO Acquire proper lib

#pragma comment( lib, LibrariesDir "freetype2/x86/freetype.lib" ) // #TODO Acquire proper lib

#ifdef _QGLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Acquire proper lib
#endif

#ifdef _QGLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib
#endif

#ifdef _QOPENAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Acquire proper lib

#ifdef _QRAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define networking library!"
#endif

#ifdef _QBULLET3
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

#ifdef _QOPENGL // #TODO Verify release lib
// #TODO 64 bit not found on local PC. Need it installed
// #pragma comment( lib, "opengl64.lib" ) // #TODO Found by Visual Studio, but explore better solution
#pragma comment( lib, "opengl32.lib" ) // #TODO Found by Visual Studio, but explore better solution
#else
#pragma warning "Define graphics library!"
#endif

#pragma comment( lib, LibrariesDir "assimp/Release/x86/assimp-vc140-mt.lib" ) // #TODO Acquire proper lib

#pragma comment( lib, LibrariesDir "freetype2/x86/freetype.lib" ) // #TODO Acquire proper lib

#ifdef _QGLEW
#pragma comment( lib, LibrariesDir "glew/glew32d.lib" ) // #TODO Acquire proper lib
#endif

#ifdef _QGLFW3
#pragma comment( lib, LibrariesDir "glfw/glfw3dll.lib" ) // #TODO Verify proper lib
#endif

#ifdef _QOPENAL
#pragma comment( lib, LibrariesDir "OpenAL/libs/Win32/OpenAL32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define audio library!"
#endif

#pragma comment( lib, LibrariesDir "pThreads/lib/x86/pthreadVC2.lib" ) // #TODO Acquire proper lib

#ifdef _QRAKNET
#pragma comment( lib, LibrariesDir "RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" ) // #TODO Acquire proper lib
#else
#pragma error "Define networking library!"
#endif

#ifdef _QBULLET3
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

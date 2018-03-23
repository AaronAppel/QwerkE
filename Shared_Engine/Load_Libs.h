#ifndef _Load_Libs_H_
#define _Load_Libs_H_

// load static and dynamic libraries
// _Platform
#if _WIN32 // x86

// graphics
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/glew/glew32d.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/glfw/glfw3.lib" )
// networking
#pragma comment( lib, "../Shared_Generic/Libraries/RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" )
// threading
#pragma comment( lib, "../Shared_Generic/Libraries/pThreads/lib/x86/pthreadVC2.lib" )
// font loading/rendering
#pragma comment( lib, "../Shared_Generic/Libraries/freetype2/freetype.lib" )

// Bullet3 physics
#if _DEBUG
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/LinearMath.lib" )
#else // Release
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/LinearMath.lib" )
#endif

#else // x64
// load x64 libraries

#endif // !_Platform

#endif // !_Load_Libs_H_
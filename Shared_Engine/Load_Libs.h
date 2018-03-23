#ifndef _Load_Libs_H_
#define _Load_Libs_H_

// load static and dynamic libraries

#pragma comment( lib, "opengl32.lib" )

// Bullet3
#if _DEBUG
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/LinearMath.lib" )
#else // Release
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/LinearMath.lib" )
#endif

#endif // !_Load_Libs_H_
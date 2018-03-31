#ifndef _Libraries_Include_H_
#define _Libraries_Include_H_

// Include/Exclude libraries based on platform, architecture, configuration
// TODO: Clean up #ifs
#ifdef _Q32bit // architecture
// platform
#ifdef _QWindows
// TODO: Add libraries

#elif _Android
#elif _Mac
#elif _Linux
#endif

#elif _Q64bit // architecture
// platform
#ifdef _QWindows
#elif _Android
#elif _Mac
#elif _Linux
#endif

#else
#pragma warning "Define Platform!"
#endif // !platform

//////////////////////////////
// FILES //
//////////////////////////////
// JSON //
// cJSON
#include "Libraries/cJSON_Interface/cJSONInterface.h"

// images //
// lodepng
#include "Libraries/lodepng/lodepng.h"
// stb
#include "../Shared_Generic/Libraries/stb_image.h"

// font loading/rendering
// freetype2
#pragma comment( lib, "../Shared_Generic/Libraries/freetype2/freetype.lib" )
#include "Libraries/freetype2/ft2build.h"
#include "../Shared_Generic/Libraries/freetype2/freetype/freetype.h"
//////////////////////////////

//////////////////////////////
// HID //
//////////////////////////////
// XBOX Controller //
// xinput
#include "Libraries/EasyXinput/EasyXinput.h"
//////////////////////////////

//////////////////////////////
// Graphics //
//////////////////////////////
// //
// open graphics library
#ifdef _Q32bit
#pragma comment( lib, "opengl32.lib" )
#else
// TODO: Is there a 64 bit openGL library?
#endif
// openGL extensions wrangler //
// GLEW
// TODO: Check for architecture and debug/release
#pragma comment( lib, "../Shared_Generic/Libraries/glew/glew32d.lib" )
#include "Libraries/glew/GL/glew.h"
// window creation //
// GLFW
// TODO: Check for architecture and debug/release
#pragma comment( lib, "../Shared_Generic/Libraries/glfw/glfw3.lib" )
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/MyGLFW/MyGLFW.h"
//////////////////////////////

//////////////////////////////
// GUIs //
//////////////////////////////
// immediate mode //
// imgui
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw_gl3.h"
// BlockMenu
#include "Libraries/Block_Menu/Block_Menu.h"
//////////////////////////////

//////////////////////////////
// threading //
//////////////////////////////
// pthreads
// TODO: Check for architecture and debug/release
#pragma comment( lib, "../Shared_Generic/Libraries/pThreads/lib/x86/pthreadVC2.lib" )
#define HAVE_STRUCT_TIMESPEC
#include "Libraries/pThreads/pthread.h"
//////////////////////////////

//////////////////////////////
// networking
//////////////////////////////
// RakNet
// TODO: Check for architecture and debug/release
#pragma comment( lib, "../Shared_Generic/Libraries/RakNet/RakNet_VS2008_DLL_Debug_Win32.lib" )
// http://www.raknet.net/raknet/manual/tutorialsample3.html
#include "Libraries/RakNet/Source/RakPeerInterface.h"
#include "Libraries/RakNet/Source/MessageIdentifiers.h"
//////////////////////////////

//////////////////////////////
// math
//////////////////////////////
// glm
#include "../Shared_Generic/Libraries/glm/glm/glm.hpp"
#include "../Shared_Generic/Libraries/glm/glm/gtc/matrix_transform.hpp"
#include "../Shared_Generic/Libraries/glm/glm/gtc/type_ptr.hpp"
/////////////////////////////

//////////////////////////////
// physics
//////////////////////////////
// Bullet3
// TODO: Check for architecture
#if _DEBUG
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Debug/LinearMath.lib" )
#else // Release
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletCollision.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/BulletDynamics.lib" )
#pragma comment( lib, "../Shared_Generic/Libraries/Bullet3/lib/Win32/Release/LinearMath.lib" )
#endif

// TODO: Add necessary headers.
// TODO: Remove unecessary headers.
// Consider creating a library for bullet projects.
#include "Libraries/Bullet3/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
/////////////////////////////

#endif // !_Libraries_Include_H_

#pragma once

#include "QF_Defines.h" // #TODO Determine which libraries to load using #defines

#ifdef _QASSIMP
#pragma warning( disable : 26495 )
#include "Libraries/assimp/config.h"
#pragma warning( default : 26495 )
#endif

#ifdef _QCJSON
#pragma warning( disable : 4267 )
#pragma warning( disable : 4244 )
#include "Libraries/cJSON/QC_cJSON.h"
#pragma warning( default : 4267)
#pragma warning( default : 4244)
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QFLATHEADGAMES
#pragma warning( disable : 4244 )
#include "QF_Tween.h"
#include "QF_TweenFuncs.h"
#pragma warning( default : 4244 )
#endif

#ifdef _QFREETYPE2
#include "Libraries/freetype2/ft2build.h"
#include "Libraries/freetype2/freetype/freetype.h"
#endif

#ifdef _QGLEW
#include "Libraries/glew/glew.h"
#endif

#ifdef _QGLFW3
#include "Libraries/glfw/QC_glfw3.h"
#endif

#ifdef _QGLM
#pragma warning( disable : 26495 )
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm//gtc/type_ptr.hpp"
#pragma warning( default : 26495 )
#endif

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#ifdef _QSPDLOG
#pragma warning( disable : 26437 ) // #TODO Switch to use push and pop
#pragma warning( disable : 26451 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26498 )
#pragma warning( disable : 6387 )
#pragma warning( disable : 6285 )
#include "Libraries/spdlog/spdlog.h"
#pragma warning( default : 26437 )
#pragma warning( default : 26451 )
#pragma warning( default : 26495 )
#pragma warning( default : 26498 )
#pragma warning( default : 6387 )
#pragma warning( default : 6285 )
#endif

// stb_image
// TODO: stb_image needs to be loaded later on, after some other ???? library
// #define STB_IMAGE_IMPLEMENTATION
// #include "Libraries/stb_image.h"
// SOIL
// TODO: get soil

// TODO: Setup OpenAL
// https://ffainelli.github.io/openal-example/
// #include "OpenAL/include/al.h"
// #include "OpenAL/include/alc.h"
// #include "OpenAL/include/efx.h"
// #include "OpenAL/include/efx-creative.h"
// #include "OpenAL/include/EFX-Util.h"
// #include "OpenAL/include/xram.h"
// #include "OpenAL/include/alut.h"
// #include "OpenAL/include/alc.h"
// #include "alut.h"
// alutinit?

#ifdef _QPTHREADS
#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC // #TODO Remove have struct pthread order dependency
#endif
#include "Libraries/pThreads/pthread.h"
#endif

// TODO: Check for architecture and debug/release
// http://www.raknet.net/raknet/manual/tutorialsample3.html

// #include "Libraries/Mirror/Source/Mirror.h"
// #include "Libraries/Mirror/Source/MirrorTypes.h"

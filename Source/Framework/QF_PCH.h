#pragma once

#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "QC_TypeDefs.h"

#ifdef _QBGFX
#include "Libraries/bx/include/bx/bx.h"
#include "Libraries/bimg/include/bimg/bimg.h"
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QFLATHEADGAMES
#pragma warning( disable : 4244 )
#include "Libraries/FlatheadGames/MyMemory.h"
#pragma warning( default : 4244 )
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

#ifdef _QGLM
#include "Libraries/Jolt/Jolt.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
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

// #NOTE: Add additional .h files below, if they rarely change
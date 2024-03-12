#pragma once

// For convenience, include commonly used files, but don't perpetually depend on this file.

// #NOTE Cannot #include "QF_PCH.h" here. Must be done in VS project settings

#define _QIgnored(x) (static_cast<void>(x))

#include "QC_CallbackFunction.h"
#include "QC_TypeDefs.h"

#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Math.h"

#ifdef _QGLEW
#include "Libraries/glew/glew.h" // #TODO Fix include order dependency with gl.h
#endif

#ifdef _QGLM
// #define GLM_FORCE_SWIZZLE // #NOTE Compiler runs out of heap space
// #include "Libraries/glm/detail/type_vec.hpp"
#include "Libraries/glm/glm.hpp"
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;
typedef glm::quat quat;
#endif

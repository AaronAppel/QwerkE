#pragma once

// #NOTE "QF_PCH.h" already included as forced include in premake build file
// Cannot #include "QF_PCH.h" here. Must be done in VS project settings

// For convenience, include commonly used files, types, aliases, etc

#define _QIGNORED(x) (static_cast<void>(x))

#include "QC_CallbackFunction.h"
#include "QC_Math.h"
#include "QC_TypeDefs.h"

#include "QF_Constants.h"
#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Paths.h"

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"

typedef glm::vec2		vec2f;
typedef glm::u16vec2	vec2u16; // #TODO Review aliasing and it's usefulness and clarity

typedef glm::vec3		vec3f;
typedef glm::vec4		vec4f;
typedef glm::mat3		mat3;
typedef glm::mat4		mat4;
typedef glm::quat		quat;
#endif

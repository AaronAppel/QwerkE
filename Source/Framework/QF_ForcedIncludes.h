#pragma once

// #NOTE "QF_PCH.h" already included as forced include in premake build file
// Cannot #include "QF_PCH.h" here. Must be done in VS project settings

// For convenience, include commonly used files, but don't perpetually depend on this file.

#define _QIGNORED(x) (static_cast<void>(x))

#include <string>
typedef std::string str; // #TODO Review if wanted or not

#include "QC_CallbackFunction.h"
#include "QC_Math.h"
#include "QC_TypeDefs.h"

#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Paths.h"

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"

typedef glm::vec2		vec2f;
typedef glm::u16vec2	vec2u16;

typedef glm::vec3		vec3f;
typedef glm::vec4		vec4f;
typedef glm::mat4		mat4;
typedef glm::quat		quat;
#endif

namespace QwerkE {

	namespace Constants {

		static constexpr char* gc_DefaultStringValue = "null";

	}

}

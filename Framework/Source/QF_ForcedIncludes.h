#pragma once

// For convenience, include commonly used files, but don't perpetually depend on this file.

// #NOTE Cannot #include "QF_PCH.h" here. Must be done in VS project settings

#define _QIGNORED(x) (static_cast<void>(x))

#include <string>
typedef std::string str; // #TODO Review if wanted or not

#include "QC_CallbackFunction.h"
#include "QC_TypeDefs.h"

#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Math.h"
#include "QF_Paths.h"

#ifdef _QGLM
// #define GLM_FORCE_SWIZZLE // #NOTE Compiler runs out of heap space
// #include "Libraries/glm/detail/type_vec.hpp"
#include "Libraries/glm/glm.hpp"

typedef glm::vec2		vec2f;
typedef glm::u16vec2	vec2u16;
// typedef glm::i32vec2	vec2s32;

typedef glm::vec3		vec3f;
typedef glm::vec4		vec4f;
typedef glm::mat4		mat4;
typedef glm::quat		quat;

// #TODO Expose more types like :
// typedef glm::f32vec2 vec2; // #TODO Make vec2 a 4 byte float by default
#endif

namespace QwerkE {

	namespace Constants {

		static constexpr char* gc_DefaultStringValue = "null";

	}

}

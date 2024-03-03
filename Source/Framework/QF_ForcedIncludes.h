#pragma once

// For convenience, include commonly used files, but don't perpetually depend on this file.

#include "QC_CallbackFunction.h"
#include "QC_TypeDefs.h"

#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Math.h"
#include "QF_Memory.h"

#define GLM_FORCE_SWIZZLE // #TODO Review build time impact

#include "Libraries/glm/glm/vec2.hpp"
typedef glm::vec2 vec2;
#include "Libraries/glm/glm/vec3.hpp"
typedef glm::vec3 vec3;
#include "Libraries/glm/glm/vec4.hpp"
typedef glm::vec4 vec4;

#include "Libraries/glm/glm/mat4x4.hpp"
typedef glm::mat4 mat4;

#include "Libraries/glm/glm/gtx/quaternion.hpp"
typedef glm::quat quat;

// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#pragma once

// #includes forced to be included throughout QwerkE

#include "QC_TypeDefs.h"

#include "QF_Math.h"
#include "QF_Memory.h"

// #define GLM_SWIZZLE // #TODO Review build time impact

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

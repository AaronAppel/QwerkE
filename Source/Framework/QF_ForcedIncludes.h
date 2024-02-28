#pragma once

// #includes forced to be included throughout QwerkE

#include "QC_TypeDefs.h"

#include "QF_Math.h"
#include "QF_Memory.h"

// #define GLM_SWIZZLE // #TODO Review build time impact

#include "Libraries/glm/glm/vec2.hpp"
typedef glm::vec2 Vec2;
#include "Libraries/glm/glm/vec3.hpp"
typedef glm::vec3 Vec3;
#include "Libraries/glm/glm/vec4.hpp"
typedef glm::vec4 Vec4;

#include "Libraries/glm/glm/mat4x4.hpp"
typedef glm::mat4 Mat4;

#include "Libraries/glm/glm/gtx/quaternion.hpp"
typedef glm::quat Quat;

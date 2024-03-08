#pragma once

/* Ensures graphics classes are included in order */
// #TODO Remove order dependency

// defines, structs, typedefs for graphics
#define Wind_CCW 0

#ifdef _QOPENGL
static const GLuint gc_NullHandleValue = 0;
#else
#error "Define rendering/graphics library!"
#endif

#include "QF_ShaderProgram.h"
#include "QF_ShaderFactory.h"

#include "QF_MeshData.h"
#include "QF_Mesh.h"
#include "QF_MeshUtilities.h"

#include "QF_CubeMap.h"
#include "QF_FrameBufferObject.h"
#include "QF_LightData.h"
#include "QF_Texture.h"

#include "QF_GraphicsHelpers.h"

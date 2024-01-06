#pragma once

/* Ensures graphics classes are included in order */

// defines, structs, typedefs for graphics
#define Wind_CCW 0

#include "QF_QwerkE_Defines.h"

#ifdef OpenGL
// GLuint gc_NullHandleValue;
#else
#pragma warning "Define rendering/graphics library!"
#endif // OpenGL

// TODO: Move enum to maybe ShaderFactory or somewhere
// that does not include a lot of other code. This is
// to avoid including more that necessary else where.
namespace QwerkE {

    enum eShaderTypes
    {
        eShader_Basic2D = 0,
        eShader_Basic3D,
        eShader_LitMaterial
        // TODO: Add max and null values?
    };

}

// My graphics classes
#include "QF_ShaderProgram.h"
#include "QF_ShaderFactory.h"

#include "QF_MeshData.h"
#include "QF_Mesh.h"
#include "QF_MeshFactory.h"
#include "QF_MeshUtilities.h"

#include "QF_CubeMap.h"
#include "QF_FrameBufferObject.h"
#include "QF_LightData.h"
#include "QF_Texture.h"

#include "QF_GraphicsHelpers.h"
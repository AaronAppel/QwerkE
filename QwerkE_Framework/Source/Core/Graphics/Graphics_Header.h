#pragma once

/* Ensures graphics classes are included in order */

// defines, structs, typedefs for graphics
#define Wind_CCW 0

#include "../../Headers/QwerkE_Defines.h"

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
#include "Shader/ShaderProgram.h"
#include "ShaderFactory/ShaderFactory.h"

#include "Mesh/MeshData.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshFactory.h"
#include "Mesh/MeshUtilities.h"

#include "DataTypes/CubeMap.h"
#include "DataTypes/FrameBufferObject.h"
#include "DataTypes/LightData.h"
#include "DataTypes/Texture.h"

#include "GraphicsUtilities/GraphicsHelpers.h"
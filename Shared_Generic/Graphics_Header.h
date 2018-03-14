#ifndef _Graphics_Header_H_
#define _Graphics_Header_H_

// glew, other opengl stuff
// all files that projects needs to draw

// 3rd party graphics libraries
#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw_gl3.h"

#include "Libraries/freetype2/ft2build.h"
#include FT_FREETYPE_H

// defines, structs, typedefs
#define Wind_CCW 0
enum eShaderTypes
{
	eShader_Basic2D = 0,
	eShader_Basic3D,
	eShader_LitMaterial
};
// my graphics classes
#include "Graphics/ShaderProgram/ShaderProgram.h"
#include "Graphics/ShaderProgram/ShaderFactory.h"

#include "Graphics/Mesh/VertexData.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshFactory.h"
#include "Graphics/Mesh/MeshUtilities.h"

#include "Graphics/Sprite/Sprite.h"

#include "Graphics/CubeMap/CubeMap.h"
#include "Graphics/FBO/FrameBufferObject.h"
#include "Graphics/Model/Model.h"

#endif // !_Graphics_Header_H_

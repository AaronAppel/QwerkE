#pragma once

#include "QF_Renderer.h"

#include <stdio.h>

#ifdef _QBGFX
#include "Libraries/bgfx/examples/common/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#include "Libraries/bgfx/include/bgfx/platform.h" // bgfx::PlatformData
#include "Libraries/bgfx/src/bgfx_p.h" // bgfx::setGraphicsDebuggerPresent
#endif

#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtx/euler_angles.hpp"
#endif

#include "QC_Time.h"

#include "QF_Window.h"

#ifdef _QBGFX
bgfx::ShaderHandle myLoadShader(const char* FILENAME)
{
	const char* shaderPath = "???";

	switch (bgfx::getRendererType()) {
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
		case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
		case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
		case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;

		case bgfx::RendererType::Noop:
		default:
			LOG_ERROR("{0} Error loading bgfx shader {1}", __FUNCTION__, FILENAME ? FILENAME : "null");
	}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char* filePath = (char*)malloc(shaderLen + fileLen + 1);

	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);
	filePath[shaderLen + fileLen] = '\0';

	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	}
	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};
static const uint16_t s_cubeTriList[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };

static const char* s_ptNames[]
{
	"Triangle List",
	"Triangle Strip",
	"Lines",
	"Line Strip",
	"Points",
};

static const uint64_t s_ptState[]
{
	UINT64_C(0), // Regular Triangles draw
	BGFX_STATE_PT_TRISTRIP,
	BGFX_STATE_PT_LINES,
	BGFX_STATE_PT_LINESTRIP,
	BGFX_STATE_PT_POINTS,
};
BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames));
#endif

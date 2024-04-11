#pragma once

#ifdef _QBGFX
#include <stdio.h>

#include "Libraries/bgfx/include/bgfx/bgfx.h"
#include "Libraries/bgfx/examples/common/bgfx_utils.h"

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

bgfx::ProgramHandle myLoadShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	const bgfx::ShaderHandle vertexShader = myLoadShader(vertexShaderFilePath);
	const bgfx::ShaderHandle fragmentShader = myLoadShader(fragmentShaderFilePath);

	return bgfx::createProgram(vertexShader, fragmentShader, false);
}

// Mesh* otherMeshLoad(bx::ReaderSeekerI* _reader, bool _ramcopy)
// {
// 	Mesh* mesh = new Mesh;
// 	mesh->load(_reader, _ramcopy);
// 	return mesh;
// }
//
// Mesh* myMeshLoad(const char* _filePath, bool _ramcopy = false)
// {
// 	static bx::FileReaderI* s_fileReader = NULL;
// 	if (bx::open(s_fileReader, _filePath))
// 	{
// 		Mesh* mesh = otherMeshLoad(s_fileReader, _ramcopy);
// 		bx::close(s_fileReader);
// 		return mesh;
// 	}
//
// 	return NULL;
// }
#endif

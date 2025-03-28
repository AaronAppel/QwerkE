#include "QF_RendererHelpers.h"

#ifdef _QBGFX
#include <stdio.h>

#include "Libraries/bgfx/include/bgfx/bgfx.h"

#include "Libraries/bx/include/bx/readerwriter.h"
#include "Libraries/bx/include/bx/file.h"

bgfx::ShaderHandle myLoadShaderName(const char* FILENAME)
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

bgfx::ShaderHandle myLoadShaderPath(const char* filePath)
{
	// #TODO Exception if file doesn't exist
	FILE* fileHandle = fopen(filePath, "rb");
	fseek(fileHandle, 0, SEEK_END);
	long fileContentsSize = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileContentsSize + 1);
	fread(mem->data, 1, fileContentsSize, fileHandle);
	mem->data[mem->size - 1] = '\0';
	fclose(fileHandle);

	return bgfx::createShader(mem);
}

bgfx::ProgramHandle myLoadShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	const bgfx::ShaderHandle vertexShader = myLoadShaderPath(vertexShaderFilePath);
	const bgfx::ShaderHandle fragmentShader = myLoadShaderPath(fragmentShaderFilePath);

	return bgfx::createProgram(vertexShader, fragmentShader, true);
}

bool FileReader::open(const bx::FilePath& _filePath, bx::Error* _err)
{
	return super::open(_filePath.getCPtr(), _err);
}

// #TODO Create some Loader() class that manages static variables, or deprecate the need
static bx::DefaultAllocator s_allocator;
bx::AllocatorI* g_allocator = &s_allocator;
static bx::FileReaderI* s_fileReader;

bgfxFramework::Mesh* myMeshLoad(const char* _filePath, bool _ramcopy)
{
	if (!s_fileReader)
	{
		s_fileReader = BX_NEW(g_allocator, FileReader);
	}

	if (bx::open(s_fileReader, _filePath))
	{
		bgfxFramework::Mesh* mesh = new bgfxFramework::Mesh();
		loadMesh(mesh, s_fileReader, _ramcopy);
		bx::close(s_fileReader);
		return mesh;
	}

	if (s_fileReader)
	{
		bx::deleteObject(g_allocator, s_fileReader);
		s_fileReader = NULL;
	}

	return NULL;
}
#endif

#pragma once

#ifdef _QBGFX
#include <stdio.h>

#include "Libraries/bgfx/include/bgfx/bgfx.h"

#ifdef _QBGFXFRAMEWORK
#include "Libraries/bgfxFramework/include/bgfxFramework/bgfx_utils.h"
#endif

#include "Libraries/bx/include/bx/readerwriter.h"
#include "Libraries/bx/include/bx/file.h"

bgfx::ShaderHandle myLoadShader(const char* FILENAME);

bgfx::ProgramHandle myLoadShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;

public:
	bool open(const bx::FilePath& _filePath, bx::Error* _err) override;
};

bgfxFramework::Mesh* myMeshLoad(const char* _filePath, bool _ramcopy = false);
#endif

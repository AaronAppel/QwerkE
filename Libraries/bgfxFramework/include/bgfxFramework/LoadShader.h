#pragma once

#include <bgfx/bgfx.h>

bgfx::ShaderHandle myLoadShader(const char* FILENAME);
bgfx::ProgramHandle myLoadShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);


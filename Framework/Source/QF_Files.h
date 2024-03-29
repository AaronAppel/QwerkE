#pragma once

#include "QF_Buffer.h"

namespace QwerkE {

    namespace Files {

        Buffer LoadFile(const char* filePath);

        Buffer LoadPng(const char* filePath, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels);

    }

}

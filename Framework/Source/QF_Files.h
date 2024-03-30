#pragma once

#include <filesystem>

typedef std::filesystem::path Path;

#include "QF_Buffer.h"

namespace QwerkE {

    namespace Files {

        bool Exists(const char* filePath);

        Buffer LoadFile(const char* filePath);
        Buffer LoadPng(const char* filePath, unsigned int* imageWidth, unsigned int* imageHeight, u16& channels);
        Buffer LoadWav(const char* filePath);

        Path FileName(const char* filePath);
        Path FileExtension(const char* filePath);

        // std::string ExplorerOpen();
        // std::string ExplorerSave();

        uPtr<char> UniqueFileName(const char* const filePath);

    }

}

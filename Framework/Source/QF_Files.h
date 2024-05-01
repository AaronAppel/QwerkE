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

        void WriteStringToFile(const char* const fileData, const char* const filePath);

        // std::string ExplorerOpen();
        // std::string ExplorerSave();

        Path UniqueFilePath(const char* const filePath);
        Path UniqueFilePath(const char* const dirPath, const char* const fileName);

    }

}

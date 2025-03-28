#pragma once

#include <filesystem>

#include "QF_Buffer.h"

namespace QwerkE {

    typedef std::filesystem::path Path;

    namespace Files {

        struct Extensions
        {
            // #TODO QF_PathDefines.h
            // For safety and convenience
            const char* const Preference = ".qpref";
            const char* const Registry = ".qreg";
            const char* const Scene = ".qscene";
            const char* const Setting = ".qsetting";
        };

        bool Exists(const char* filePath);

        Buffer LoadFile(const char* filePath);
        Buffer LoadPng(const char* filePath, unsigned int* imageWidth, unsigned int* imageHeight, u16& channels);
        Buffer LoadWav(const char* filePath);

        Path FileName(const char* filePath);
        Path FileExtension(const char* filePath);

        void WriteStringToFile(const char* const fileData, const char* const filePath);

        std::string ExplorerOpen(const char* filter);
        std::string ExplorerSave(const char* filter);

        Path UniqueFilePath(const char* const filePath);
        Path UniqueFilePath(const char* const dirPath, const char* const fileName);

    }

}

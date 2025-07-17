#pragma once

#include <filesystem>

#include "QF_Buffer.h"

namespace QwerkE {

    typedef std::filesystem::path Path;

    struct SoundFile;

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
        SoundFile* LoadWav(const char* filePath);

        Path FileName(const char* filePath);
        Path FileNameNoExt(const char* const filePath);
        Path FileExtension(const char* filePath);

        void CreateEmptyFile(const char* const filePath);

        void WriteStringToFile(const char* const fileData, const char* const filePath);

        std::string ExplorerOpen(const char* filter, const char* dirPath = nullptr);
        std::string ExplorerSave(const char* filter, const char* dirPath = nullptr);

        Path UniqueFilePath(const char* const filePath);
        Path UniqueFilePath(const char* const dirPath, const char* const fileName);

    }

}

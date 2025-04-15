#pragma once

#include <filesystem> // For std::filesystem::directory_iterator #TODO Review smaller include

namespace QwerkE {

    namespace Directory {

        typedef std::filesystem::path Path;

        bool Exists(const char* dirPath);

        std::vector<const char*> ListFiles(const char* dirPath, const char* const extension = nullptr, const bool inclusiveExtension = true);

        std::filesystem::directory_iterator ListDir(const char* directoryPath);

    }

}

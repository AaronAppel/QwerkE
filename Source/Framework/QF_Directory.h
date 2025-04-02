#pragma once

#include <filesystem> // For std::filesystem::directory_iterator #TODO Review smaller include

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath);

        std::filesystem::directory_iterator ListDir(const char* directoryPath);

    }

}

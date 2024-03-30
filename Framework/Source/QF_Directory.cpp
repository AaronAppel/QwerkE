#include "QF_Directory.h"

#include <filesystem>

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath)
        {
            return false;
        }

        std::vector<std::string> ListDir(const char* directoryPath)
        {
            // https://en.cppreference.com/w/cpp/filesystem/directory_iterator
            const bool recursive = false;
            return std::vector<std::string>();
        }

    }

}

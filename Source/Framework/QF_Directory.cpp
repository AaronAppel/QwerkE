#include "QF_Directory.h"

#include <filesystem>

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath)
        {
            ASSERT(false, "Not implemented!");
            return std::filesystem::status(dirPath).type() == std::filesystem::file_type::directory;
        }

        std::vector<std::string> ListDir(const char* directoryPath)
        {
            ASSERT(false, "ListDir Code not implemented yet!");

            // https://en.cppreference.com/w/cpp/filesystem/directory_iterator
            const bool recursive = false;
            return std::vector<std::string>();
        }

    }

}

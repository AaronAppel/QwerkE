#include "QF_Directory.h"

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath)
        {
            return std::filesystem::status(dirPath).type() == std::filesystem::file_type::directory;
        }

        std::filesystem::directory_iterator ListDir(const char* directoryPath)
        {
            // #TODO Test with bad directory path
            return std::filesystem::directory_iterator(directoryPath);
        }

    }

}

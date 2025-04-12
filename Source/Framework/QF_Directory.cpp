#include "QF_Directory.h"

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath)
        {
            return std::filesystem::status(dirPath).type() == std::filesystem::file_type::directory;
        }

        std::filesystem::directory_iterator ListDir(const char* dirPath)
        {
            // #TODO Test with bad directory path
            if (!Exists(dirPath))
            {
                return std::filesystem::directory_iterator();
            }
            return std::filesystem::directory_iterator(dirPath);
        }

    }

}

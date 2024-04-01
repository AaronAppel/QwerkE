#pragma once

#include <string>
#include <vector>

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath);

        std::vector<std::string> ListDir(const char* directoryPath);

    }

}

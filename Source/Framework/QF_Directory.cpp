#include "QF_Directory.h"

#include <vector>

namespace QwerkE {

    namespace Directory {

        bool Exists(const char* dirPath)
        {
            return std::filesystem::status(dirPath).type() == std::filesystem::file_type::directory;
        }

        std::vector<const char*> ListFiles(const char* dirPath, const char* const extension, const bool inclusiveExtension)
        {
            // #TODO Test with bad directory path

            std::vector<const char*> fileNames;
            if (!Exists(dirPath))
            {
                return fileNames;
            }
            fileNames.reserve(20);

            std::filesystem::directory_iterator dirIterator(dirPath);
            for (const auto& directoryEntry : dirIterator)
            {
                const Path& path = directoryEntry.path();
                if (extension)
                {
                    // #TODO Ignore extensions if no extension?
                    // path.has_extension();

                    const bool matchingExtensions = !strcmp(path.extension().generic_string().c_str() + 1, extension);
                    if ((matchingExtensions && !inclusiveExtension) ||
                        (!matchingExtensions && inclusiveExtension))
                    {
                        continue;
                    }
                }

                fileNames.emplace_back(strdup(path.filename().u8string().c_str()));
            }
            return fileNames;
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

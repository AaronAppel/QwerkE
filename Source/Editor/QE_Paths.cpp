#include "QE_Paths.h"

#include <filesystem>
#include <string>

#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		std::string s_RepoRootDir = "";
		constexpr char* s_RepoRootSearchKey = "bin"; // #NOTE Requires "bin" directory to be part of the exe path

		std::string RepoRootDir()
		{
			if (!s_RepoRootDir.empty())
			{
				return s_RepoRootDir;
			}

			Path pathToExe = GetExePath();

			while (!pathToExe.empty())
			{
				if (strcmp(s_RepoRootSearchKey, pathToExe.filename().string().c_str()) == 0)
				{
					s_RepoRootDir = pathToExe.parent_path().string();
					break;
				}

				if (!pathToExe.has_parent_path())
				{
					break;
				}

				pathToExe = pathToExe.parent_path();
			}

			ASSERT(!s_RepoRootDir.empty(), "Error finding repo root directory!");
			return s_RepoRootDir;
		}

		std::string ScriptsDir()
		{
			return RepoRootDir() + "\\Build"; // #TODO Update to "\\Scripts"
		}

		std::string Script(const char* scriptFileName)
		{
			return ScriptsDir() + "\\" + scriptFileName;
		}
	}

}

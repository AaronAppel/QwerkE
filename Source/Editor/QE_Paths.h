#pragma once

namespace QwerkE {

	namespace Paths {

		std::string RepoRootDir();

		std::string ScriptsDir();
		std::string Script(const char* scriptFileName); // #TODO Improve ambiguity with game or other engine scripts

	}

}

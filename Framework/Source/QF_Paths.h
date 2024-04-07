#pragma once

#include <string>

namespace QwerkE {

	namespace Paths {

		std::string RootDir();

		std::string AssetsDir();

		// #TODO Look at using std::strings or string views instead of const char*
		std::string NullAsset(const char* sceneFileName);

		std::string Project(const char* projectFileName);

		std::string ScenesDir();
		std::string Scene(const char* sceneFileName);

		std::string Schematic(const char* schematicFileName);

		std::string Settings(const char* settingsFileName);

	}

}

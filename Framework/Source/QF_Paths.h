#pragma once

#include <filesystem>
#include <string>

typedef std::filesystem::path Path;

namespace QwerkE {

	namespace Paths {

		// #TODO Look at using std::strings or string views instead of const char*

		void SetNullAssetsDir(std::string newRootDir);
		void SetAssetsDir(std::string newAssetsDir);

		std::string RootDir();

		std::string AssetsDir();

		std::string NullAssetsDir();
		std::string NullAsset(const char* sceneFileName);

		std::string ProjectsDir();
		std::string Project(const char* projectFileName);

		std::string MeshesDir();
		std::string Mesh(const char* meshFileName);

		std::string ScenesDir();
		std::string Scene(const char* sceneFileName);

		std::string Schematic(const char* schematicFileName);

		std::string Settings(const char* settingsFileName);

		std::string ShadersDir();
		std::string Shader(const char* shaderFileName);

	}

}

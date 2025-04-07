#pragma once

#include <filesystem>
#include <string>

typedef std::filesystem::path Path;

namespace QwerkE {

	namespace Paths {

		// #TODO Look at using std::strings or string views instead of const char*

		void SetExePath(const std::string& newExePath);
		void SetAssetsDir(const std::string& newAssetsDir);
		void SetNullAssetsDir(const std::string& newRootDir);

#ifdef _QDEBUG
		void SetRepoRootDir(const std::string&);
		const std::string& RepoRootDir();
#endif // _QDEBUG

		const std::string& ExeDir();

		const std::string& AssetsDir();

		std::string FontsDir();
		std::string Font(const char* fontFileName);

		std::string MeshesDir();
		std::string Mesh(const char* meshFileName);

		const std::string& NullAssetsDir();
		std::string NullAsset(const char* sceneFileName);

		std::string ProjectsDir();
		std::string Project(const char* projectFileName);

		std::string ScenesDir();
		std::string Scene(const char* sceneFileName);

		std::string SchematicsDir();
		std::string Schematic(const char* schematicFileName);

		std::string SettingsDir();
		std::string Setting(const char* settingsFileName);

		std::string ShadersDir();
		std::string Shader(const char* shaderFileName);

	}

}

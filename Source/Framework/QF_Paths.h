#pragma once

#include <filesystem>
#include <string>

typedef std::filesystem::path Path;

namespace QwerkE {

	namespace Paths {

		// #TODO Look at using std::strings or string views instead of const char*

		void SetExePath(std::string newExePath);
		void SetAssetsDir(std::string newAssetsDir);
		void SetNullAssetsDir(std::string newRootDir);

		std::string RootDir();

		std::string AssetsDir();

		std::string FontsDir();
		std::string Font(const char* fontFileName);

		std::string MeshesDir();
		std::string Mesh(const char* meshFileName);

		std::string NullAssetsDir();
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

		std::string StylesDir();
		std::string Style(const char* shaderFileName);

	}

}

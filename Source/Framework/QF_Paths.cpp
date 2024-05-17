#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		static std::string s_ExePath = "";
		static std::string s_AssetsDir = "Assets";
		static std::string s_NullAssetsDir = "Assets";

		void SetExePath(std::string newExePath)
		{
			s_ExePath = newExePath;

			Path exePath = s_ExePath; // #TODO Assume assets folder exists with .exe file
			exePath = exePath.parent_path();
			exePath = exePath / "Assets";
			SetAssetsDir(exePath.string());
		}

		void SetAssetsDir(std::string newAssetsDir)
		{
			s_AssetsDir = newAssetsDir;
		}

		void SetNullAssetsDir(std::string newNullAssetsDir)
		{
			s_NullAssetsDir = newNullAssetsDir;
		}

		std::string AssetsDir()
		{
			return s_AssetsDir;
		}

		std::string NullAssetsDir()
		{
			return s_NullAssetsDir;
		}

		std::string NullAsset(const char* sceneFileName)
		{
			// #TODO Null directory is in build settings. Should be improved
			return s_NullAssetsDir + "\\" + sceneFileName;
		}

		std::string ProjectsDir()
		{
			return AssetsDir() + "\\Projects";
		}

		std::string Project(const char* projectFileName)
		{
			return ProjectsDir() + "\\" + projectFileName;
		}

		std::string MeshesDir()
		{
			return AssetsDir() + "\\Meshes";
		}

		std::string Mesh(const char* meshFileName)
		{
			return MeshesDir() + "\\" + meshFileName;
		}

		std::string ScenesDir()
		{
			return AssetsDir() + "\\Scenes";
		}

		std::string Scene(const char* sceneFileName)
		{
			return ScenesDir() + "\\" + sceneFileName;
		}

		std::string SchematicsDir()
		{
			return AssetsDir() + "\\Schematics";
		}

		std::string Schematic(const char* schematicFileName)
		{
			return SchematicsDir() + "\\" + schematicFileName;
		}

		std::string SettingsDir()
		{
			return AssetsDir() + "\\Settings";
		}

		std::string Setting(const char* settingsFileName)
		{
			return SettingsDir() + "\\" + settingsFileName;
		}

		std::string ShadersDir()
		{
			return AssetsDir() + "\\Shaders";
		}

		std::string Shader(const char* shaderFileName)
		{
			// #TODO Should the RendererType be used to find a shader sub directory (dx11/, openGL/, etc?
			return ShadersDir() + "\\" + shaderFileName;
		}

	}

}

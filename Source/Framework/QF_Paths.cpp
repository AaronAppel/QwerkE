#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		static std::string s_ExePath = "";
		static std::string s_AssetsDir = "Assets";
		static std::string s_NullAssetsDir = "Assets";

		void SetExePath(const std::string& newExePath)
		{
			// #TODO Validation
			s_ExePath = newExePath;

			Path assetsPath = s_ExePath; // #TODO Assume assets folder exists with .exe file
			assetsPath = assetsPath.parent_path();
			assetsPath = assetsPath / "Assets";
			SetAssetsDir(assetsPath.string());

#ifdef _QDEBUG
			Path repoRootDirPath = s_ExePath; // #TODO Assume repor root is named EngineName
			while (!repoRootDirPath.empty())
			{
				repoRootDirPath = repoRootDirPath.parent_path();
				if (repoRootDirPath.filename() == EngineName)
				{
					SetRepoRootDir(repoRootDirPath.generic_string());
					break;
				}
			}
#endif // _QDEBUG
		}

		void SetAssetsDir(const std::string& newAssetsDir)
		{
			// #TODO Validation
			s_AssetsDir = newAssetsDir;
		}

		void SetNullAssetsDir(const std::string& newNullAssetsDir)
		{
			// #TODO Validation
			s_NullAssetsDir = newNullAssetsDir;
		}

#ifdef _QDEBUG
		static std::string s_RepoRootDir = "";
		void SetRepoRootDir(const std::string& newRepoRootDir)
		{
			// #TODO Validation
			s_RepoRootDir = newRepoRootDir;
		}

		const std::string& RepoRootDir()
		{
			return s_RepoRootDir;
		}
#endif // _QDEBUG

		const std::string& ExeDir()
		{
			return s_ExePath;
		}

		const std::string& AssetsDir()
		{
			return s_AssetsDir;
		}

		std::string FontsDir()
		{
			return AssetsDir() + "\\Fonts";
		}

		std::string Font(const char* fontFileName)
		{
			return FontsDir() + "\\" + fontFileName;
		}

		std::string MeshesDir()
		{
			return AssetsDir() + "\\Meshes";
		}

		std::string Mesh(const char* meshFileName)
		{
			return MeshesDir() + "\\" + meshFileName;
		}

		const std::string& NullAssetsDir()
		{
			return s_NullAssetsDir;
		}

		std::string NullAsset(const char* nullFileName)
		{
			// #TODO Null directory is in build settings. Should be improved
			return s_NullAssetsDir + "\\" + nullFileName;
		}

		std::string ProjectsDir()
		{
			return AssetsDir() + "\\Projects";
		}

		std::string Project(const char* projectFileName)
		{
			return ProjectsDir() + "\\" + projectFileName;
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

#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		static std::string s_NullAssetsDir = "..\\";
		static std::string s_AssetsDir = "..\\";

		void SetNullAssetsDir(std::string newRootDir)
		{
			s_NullAssetsDir = newRootDir;
		}

		void SetAssetsDir(std::string newAssetsDir)
		{
			s_AssetsDir = newAssetsDir;
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

		std::string Schematic(const char* schematicFileName)
		{
			return AssetsDir() + "\\Schematics\\" + schematicFileName;
		}

		std::string Settings(const char* settingsFileName)
		{
			return AssetsDir() + "\\Settings\\" + settingsFileName;
		}

		std::string ShadersDir()
		{
			return AssetsDir() + "\\Shaders";
		}

		std::string Shader(const char* shaderFileName)
		{
			return ShadersDir() + "\\" + shaderFileName;
		}

	}

}

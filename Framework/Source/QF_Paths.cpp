#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		std::string RootDir()
		{
			return std::string(WorkspaceRootDir);
		}

		std::string AssetsDir()
		{
			return RootDir() + "Game\\Assets\\";
		}

		std::string NullAsset(const char* sceneFileName)
		{
			return RootDir() + "Editor\\Assets\\" + sceneFileName;
		}

		std::string Project(const char* projectFileName)
		{
			return AssetsDir() + "Projects\\" + projectFileName;
		}

		std::string MeshesDir()
		{
			return AssetsDir() + "Meshes\\";
		}

		std::string Mesh(const char* meshFileName)
		{
			return MeshesDir() + meshFileName;
		}

		std::string ScenesDir()
		{
			return AssetsDir() + "Scenes\\";
		}

		std::string Scene(const char* sceneFileName)
		{
			return ScenesDir() + sceneFileName;
		}

		std::string Schematic(const char* schematicFileName)
		{
			return AssetsDir() + "Schematics\\" + schematicFileName;
		}

		std::string Settings(const char* settingsFileName)
		{
			return AssetsDir() + "Settings\\" + settingsFileName;
		}

		std::string ShadersDir()
		{
			return AssetsDir() + "Shaders\\";
		}

		std::string Shader(const char* shaderFileName)
		{
			return ShadersDir() + shaderFileName;
		}

	}

}

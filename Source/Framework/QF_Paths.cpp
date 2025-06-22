#include "QF_Paths.h"

namespace QwerkE {

	namespace Paths {

		static std::string s_ExePath = "";
		static std::string s_AssetsDir = "Assets";		// #TODO Review paths
		static std::string s_NullAssetsDir = "Assets";  // #TODO Review paths

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

		const std::string& GetExePath()
		{
			return s_ExePath;
		}

		std::string AssetsDir()
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

		std::string NullAssetsDir()
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
			return ShadersDir() + "\\" + shaderFileName;
		}

		std::string ShadersBinDir()
		{
			return ShadersDir() + "\\Bin";
		}

		std::string ShaderBin(const char* shaderBinaryFileName)
		{
			ASSERT(shaderBinaryFileName, "Invalid shaderBinaryFileName argument!");

			std::string returnPath = ShadersBinDir();

			switch (bgfx::getRendererType()) {
			case bgfx::RendererType::Direct3D11:
			case bgfx::RendererType::Direct3D12: returnPath += "\\dx11\\";  break;
			case bgfx::RendererType::Gnm:        returnPath += "\\pssl\\";  break;
			case bgfx::RendererType::Metal:      returnPath += "\\metal\\"; break;
			case bgfx::RendererType::OpenGL:     returnPath += "\\glsl\\";  break;
			case bgfx::RendererType::OpenGLES:   returnPath += "\\essl\\";  break;
			case bgfx::RendererType::Vulkan:     returnPath += "\\spirv\\"; break;

			case bgfx::RendererType::Noop:
			default:
				LOG_ERROR("{0}(): Error loading binary shader {1}", __FUNCTION__, shaderBinaryFileName ? shaderBinaryFileName : "null");
			}

			return returnPath + shaderBinaryFileName;
		}

		std::string StylesDir()
		{
			return AssetsDir() + "\\Styles";
		}

		std::string Style(const char* stylesFileName)
		{
			return StylesDir() + "\\" + stylesFileName;
		}

	}

}

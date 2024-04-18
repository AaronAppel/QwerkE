#include "QF_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
// #include "QF_Serialization.h"

namespace QwerkE {

	namespace Settings {

		constexpr char* s_ImGuiStyleFileName = "imguiStyle.style";

		constexpr char* s_NullEngineSettings = "null_settings.qeng";
		constexpr char* s_NullRendererSettings = "null_settings.qren";
		constexpr char* s_NullUserSettings = "null_settings.quser";

		EngineSettings s_engineSettings;
		UserSettings s_userSettings;
		RendererSettings s_rendererSettings;

		EngineSettings& GetEngineSettings()
		{
			return s_engineSettings;
		}

		UserSettings& GetUserSettings()
		{
			return s_userSettings;
		}

		RendererSettings& GetRendererSettings()
		{
			return s_rendererSettings;
		}

		const char* GetStyleFileName()
		{
			return s_ImGuiStyleFileName;
		}

		void LoadEngineSettings(const std::string& engineSettingsFileName)
		{
			std::string engineSettingsFilePath = Paths::Settings(engineSettingsFileName.c_str());
			s_engineSettings.isDirty = false;
		}

		void SaveEngineSettings(/* #TODO Take in file name */)
		{
			s_engineSettings.isDirty = false;
		}

		void LoadUserSettings(const std::string& userSettingsFileName)
		{
			std::string userSettingsFilePath = Paths::Settings(userSettingsFileName.c_str());
			if (userSettingsFilePath.empty() || !Files::Exists(userSettingsFilePath.c_str()))
			{
				userSettingsFilePath = Paths::NullAsset(null_config);
			}
			s_userSettings.isDirty = false;
		}

		void SaveUserSettings()
		{
			s_userSettings.isDirty = false;
		}

		void LoadRendererSettings(const std::string& rendererSettingsFileName)
		{
			std::string rendererSettingsFilePath = Paths::Settings(rendererSettingsFileName.c_str());
			if (rendererSettingsFileName.empty() || rendererSettingsFilePath.empty() ||
				!Files::Exists(rendererSettingsFilePath.c_str()))
			{
				rendererSettingsFilePath = Paths::NullAsset(null_config);
			}
			s_rendererSettings.isDirty = false;
		}

		void SaveRendererSettings()
		{
			s_rendererSettings.isDirty = false;
		}

	}

}

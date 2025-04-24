#include "QE_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

namespace QwerkE {

	namespace Settings {

		constexpr char* s_ImGuiStyleFileName = "Default.style"; // #TODO Review default settings files list
		constexpr char* s_ImGuiStyleFileName2 = "Vekor64.style"; // #TODO Move to QE_Settings.h::UserSettings

		UserSettings s_userSettings;
		RendererSettings s_rendererSettings;

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

		const char* GetStyleFileName2()
		{
			return s_ImGuiStyleFileName2;
		}

		void LoadUserSettings(const std::string& userSettingsFileName)
		{
			std::string userSettingsFilePath = Paths::Setting(userSettingsFileName.c_str());
			if (userSettingsFileName.empty() || userSettingsFilePath.empty() ||
				!Files::Exists(userSettingsFilePath.c_str()))
			{
				userSettingsFilePath = Paths::NullAsset("UserSettings.qpref"); // #TODO Add to path/file defines
			}
			Serialize::FromFile(userSettingsFilePath.c_str(), s_userSettings);
			s_userSettings.isDirty = false;
		}

		void SaveUserSettings()
		{
			Serialize::ToFile(s_userSettings, Paths::Setting("Aaron.qpref").c_str());
		}

		void LoadRendererSettings(const std::string& rendererSettingsFileName)
		{
			std::string rendererSettingsFilePath = Paths::Setting(rendererSettingsFileName.c_str());
			if (rendererSettingsFileName.empty() || rendererSettingsFilePath.empty() ||
				!Files::Exists(rendererSettingsFilePath.c_str()))
			{
				rendererSettingsFilePath = Paths::NullAsset(null_config);
			}
			Serialize::FromFile(rendererSettingsFilePath.c_str(), s_rendererSettings);
			s_rendererSettings.isDirty = false;
		}

		void SaveRendererSettings()
		{
			// #TODO Serialize to file
			s_rendererSettings.isDirty = false;
		}

	}

}

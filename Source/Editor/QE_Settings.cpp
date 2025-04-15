#include "QE_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

namespace QwerkE {

	namespace Settings {

		constexpr char* s_ImGuiStyleFileName = "Default.style"; // #TODO Review default settings files list

		// #TODO Should this be framework settings? Or maybe just game and editor have settings to pass to the framework?
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
			std::string engineSettingsFilePath = Paths::Setting(engineSettingsFileName.c_str());
			if (!Files::Exists(engineSettingsFilePath.c_str()))
				return;

			Serialize::FromFile(engineSettingsFilePath.c_str(), s_engineSettings);
			s_engineSettings.isDirty = false;
		}

		void SaveEngineSettings(const std::string& engineSettingsFileName)
		{
			const std::string engineSettingsFilePath = Paths::Setting(engineSettingsFileName.c_str());
			Serialize::ToFile(s_engineSettings, engineSettingsFileName.c_str());
			s_engineSettings.isDirty = false;
		}

		void SaveEngineSettings()
		{
			// #TODO Store current/latest engine settings file and serialize to that file?
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

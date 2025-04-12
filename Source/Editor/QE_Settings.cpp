#include "QE_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

namespace QwerkE {

	namespace Settings {

		constexpr char* s_ImGuiStyleFileName = "imguiStyle.style";

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

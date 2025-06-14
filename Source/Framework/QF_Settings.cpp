#include "QF_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

namespace QwerkE {

	namespace Settings {

		EngineSettings s_engineSettings;

		EngineSettings& GetEngineSettings()
		{
			return s_engineSettings;
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
			SaveEngineSettings(Paths::Setting("Editor.qsetting")); // #TODO Editor or framework settings?
		}

	}

}

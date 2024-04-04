#include "QF_Settings.h"

// #include "QF_PathDefines.h"
#include "QF_Log.h"
// #include "QF_Serialization.h"

namespace QwerkE {

	namespace Settings {

		EngineSettings s_engineSettings;
		ProjectSettings s_projectSettings;
		UserSettings s_userSettings;
		RendererSettings s_rendererSettings;

		const char* GetStyleFileName()
		{
			return "imguiStyle.style";
		}

		void LoadEngineSettings(/* #TODO Take in file path */)
		{
			// LoadEngineSettings(configFilePath);
		}

		void LoadEngineSettings(const std::string& configFilePath)
		{
			s_engineSettings.isDirty = false;
		}

		void SaveEngineSettings(/* #TODO Take in file path */)
		{
			s_engineSettings.isDirty = false;
		}

		void LoadProjectSettings(const std::string& projectSettingsFilePath)
		{
			s_projectSettings.isDirty = false;
		}

		void SaveProjectSettings()
		{
		}

		void SaveProjectSettings(const std::string& projectSettingsFilePath)
		{
			s_projectSettings.isDirty = false;
		}

		void LoadUserSettings(const std::string& userSettingsFilePath)
		{
			s_userSettings.isDirty = false;
		}

		void SaveUserSettings()
		{
			s_userSettings.isDirty = false;
		}

		void LoadRendererSettings(const std::string& rendererSettingsFilePath)
		{
			s_rendererSettings.isDirty = false;
		}

		void SaveRendererSettings()
		{
			s_rendererSettings.isDirty = false;
		}

		EngineSettings& GetEngineSettings()
		{
			return s_engineSettings;
		}

		ProjectSettings& GetProjectSettings()
		{
			return s_projectSettings;
		}

		UserSettings& GetUserSettings()
		{
			return s_userSettings;
		}

		RendererSettings& GetRendererSettings()
		{
			return s_rendererSettings;
		}

	}

}

#include "QE_Settings.h"

#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

namespace QwerkE {

	namespace Settings {

		constexpr char* s_ImGuiStyleFileName = "Default.style"; // #TODO Review default settings files list
		constexpr char* s_ImGuiStyleFileName2 = "PurpleComfy.style"; // #TODO Move to QE_Settings.h::UserSettings
		constexpr char* s_EditorSettingsFileName = "Editor.qsetting"; // #TODO Move to QE_Settings.h::UserSettings

		EditorSettings s_editorSettings;
		RendererSettings s_rendererSettings;

		const char* GetStyleFileName()
		{
			return s_ImGuiStyleFileName;
		}

		const char* GetStyleFileName2()
		{
			return s_ImGuiStyleFileName2;
		}

		void LoadEditorSettings()
		{
			std::string editorSettingsFilePath = Paths::Setting(s_EditorSettingsFileName);
			if (Files::Exists(editorSettingsFilePath.c_str()))
			{
				Serialize::FromFile(editorSettingsFilePath.c_str(), s_editorSettings);
			}
			else
			{
				SaveEditorSettings();
				if (Files::Exists(editorSettingsFilePath.c_str()))
				{
					LOG_WARN("Missing editor settings file created: {0}", s_EditorSettingsFileName);
				}
				else
				{
					LOG_WARN("Error creating editor settings file: {0}", s_EditorSettingsFileName);
				}
			}
		}

		void SaveEditorSettings()
		{
			Serialize::ToFile(s_editorSettings, Paths::Setting(s_EditorSettingsFileName).c_str());
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

		EditorSettings& GetEditorSettings()
		{
			return s_editorSettings;
		}

		RendererSettings& GetRendererSettings()
		{
			return s_rendererSettings;
		}

	}

}

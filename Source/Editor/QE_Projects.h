#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "QC_Guid.h"

#include "QF_Constants.h"

namespace QwerkE {

    struct ProjectsData
    {
        std::string LastOpenedProjectFileName = Constants::gc_DefaultStringValue; // #TODO m_?
        std::vector<std::string> PreviousProjectFileNames; // #TODO m_?
    };

    // #TODO Project is an editor concept
    struct Project
    {
        bool isDirty = false; // #TODO isDirty should be an editor only state, so move to editor domain
        bool isLoaded = false;

        std::string projectFileName = Constants::gc_DefaultStringValue;
        std::string projectName = Constants::gc_DefaultStringValue;

        std::string startUpSceneName = Constants::gc_DefaultStringValue; // #TODO Could replace with GUID, or pair for human readability

        std::string projectImGuiIniFileName = "imgui.ini";

        // Referenced Scenes
        std::unordered_map<GUID, std::string> scenesList; // #TODO Review if scene name strings are really needed

        // Settings for: Physics, Audio, Graphics, etc
        // struct ConfiguredGameKeys
        // {
        //     eKeys action1 = QKey::eKeys_MAX; // QKey::eKeys_K;
        //     const char* resetScene = gc_DefaultStringValue;
        // };
        const char* lastOpenedEngineSettingsFileName = Constants::gc_DefaultStringValue;
    };

    namespace Projects {

        void Initialize();
        void Shutdown();

        ProjectsData& GetProjectsData();
        Project& CurrentProject();

        void LoadProject(const std::string& projectSettingsFilePath);
        void LoadProjectFromExplorer();
        void UnloadCurrentProject();

        void SaveProject();
        void SaveProjectToFile(const std::string& projectSettingsFilePath);
        void SaveProjectFromExplorer();

    }

}

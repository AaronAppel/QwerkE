#pragma once

namespace QwerkE {

    struct ProjectsData
    {
        std::string LastOpenedProjectFileName = Constants::gc_DefaultStringValue;
        std::vector<std::string> PreviousProjectFileNames;
    };

    // #TODO Project is an editor concept
    struct Project
    {
        bool isDirty = false; // #TODO isDirty should be an editor only state, so move to editor domain
        bool isLoaded = false;

        std::string projectFileName = Constants::gc_DefaultStringValue;
        std::string projectName = Constants::gc_DefaultStringValue;

        std::string startUpSceneName = Constants::gc_DefaultStringValue;

        // Referenced Scenes
        std::vector<std::string> sceneFileNames;

        // Settings for: Physics, Audio, Graphics, etc
        // struct ConfiguredGameKeys
        // {
        //     eKeys action1 = eKeys::eKeys_MAX; // eKeys::eKeys_K;
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

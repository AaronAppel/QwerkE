#pragma once

namespace QwerkE {

    struct Project
    {
        bool isDirty = false; // #TODO isDirty should be an editor only state, so move to editor domain
        bool isLoaded = false;

        const char* projectName = Constants::gc_DefaultStringValue;
        const char* assetsDirPath = Constants::gc_DefaultStringValue;

        // Referenced Scenes
        std::vector<std::string> sceneFileNames;

        // struct ConfiguredGameKeys
        // {
        //     eKeys action1 = eKeys::eKeys_MAX; // eKeys::eKeys_K;
        //     const char* resetScene = gc_DefaultStringValue;
        // };
    };

    namespace Projects {

        Project& CurrentProject();

        void LoadProject(const std::string& projectSettingsFilePath);

        void SaveProject();
    }

}

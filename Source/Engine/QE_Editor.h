#pragma once

namespace QwerkE {

    class EntityEditor;

    // #FEATURE F0002 obsidian://open?vault=Documentation&file=Development%2FFeatures%2FFeature%20Tickets%2FF0002

    class Editor
    {
    public:
        static void Initialize();
        static void Shutdown();

        static void NewFrame() {}
        static void Update() {}
        static void Draw();

        static void ToggleEditorUi();

        static EntityEditor* GetEntityEditor();

    protected:
    };

}

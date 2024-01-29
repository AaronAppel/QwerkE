#pragma once

namespace QwerkE {

    class EntityEditor;

    // #FEATURE F0002

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

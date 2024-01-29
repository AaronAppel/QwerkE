#pragma once

namespace QwerkE {

    class EntityEditor;

    // #TODO Create Obsidian feature ticket/note
    // FEATURE: Add cross window panel communication to support features such as
    // drag and drop assets, trigger callbacks, input, etc.
    // Or just stop using editor subclasses and switch to base functions only.

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

#pragma once

namespace QwerkE {

    class EntityEditor;

    // #FEATURE F0002

	namespace Editor {

        void Initialize();
        void Shutdown();

        void Draw();

        void ToggleEditorUi();

        EntityEditor* GetEntityEditor();

	}

}

#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        class EditorWindowStylePicker : public EditorWindow
        {
        public:
            EditorWindowStylePicker() : EditorWindow("Style Picker") { }

        private:
            void DrawInternal() override;
		};

	}

}
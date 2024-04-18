#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        class EditorWindowStylePicker : public EditorWindow
        {
        public:
            EditorWindowStylePicker(GUID guid = GUID()) : EditorWindow("Style Picker", EditorWindowTypes::StylePicker, guid) { }

        private:
            void DrawInternal() override;
		};

	}

}
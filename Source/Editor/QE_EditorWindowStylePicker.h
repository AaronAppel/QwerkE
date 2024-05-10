#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        class EditorWindowStylePicker : public EditorWindow
        {
        public:
            EditorWindowStylePicker(GUID guid = GUID()) : EditorWindow("Style Picker", EditorWindowTypes::StylePicker, guid) { }

        private:
            void DrawInternal() override;

            MIRROR_PRIVATE_MEMBERS

            bool m_ShowMoreInfo = false;
            float m_UiScaling = .8f;

            bool m_Edited = false;
		};

	}

}
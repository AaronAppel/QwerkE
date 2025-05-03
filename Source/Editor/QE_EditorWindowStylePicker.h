#pragma once

#include <vector>

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        class EditorWindowStylePicker : public EditorWindow
        {
        public:
            EditorWindowStylePicker(GUID guid = GUID()) : EditorWindow("Style Picker", EditorWindowTypes::StylePicker, guid)
            {
                m_StyleFiles.reserve(30);
            }

            ~EditorWindowStylePicker();

        private:
            void DrawInternal() override;

            MIRROR_PRIVATE_MEMBERS

            bool m_ShowMoreInfo = false;
            float m_UiScaling = .8f;
            float m_FontScale = 1.f;

            bool m_Edited = false;

            s32 m_CurrentStyleFileIndex = 0;
            std::vector<const char*> m_StyleFiles;
		};

	}

}
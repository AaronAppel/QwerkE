#pragma once

#include "QE_Editor.h" // #TODO Review removal
#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowMenuBar : public EditorWindow
		{
		public:
            EditorWindowMenuBar(GUID guid) :
                EditorWindow("MenuBar",
                    EditorWindowTypes::MenuBar,
                    guid,
                    (EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Singleton))
            {
                m_MinimumWidth = 0.f;
                m_MinimumHeight = 0.f;
            }

            ~EditorWindowMenuBar();

            bool IsUnique() override { return true; }

		private:
            void DrawInternal() override;

            float m_TextScale = 1.20f;
            bool m_CalculateFps = true;

            s32 m_CurrentStyleFileIndex = 0;
            std::vector<const char*> m_StyleFiles;
		};

	}

}
#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowSceneView : public EditorWindow
		{
		public:
			EditorWindowSceneView(std::string windowName);

			void SetViewId(u8 viewId) { m_ViewId = viewId; }

		private:
			void DrawInternal() override;

			u8 m_ViewId = 0;
		};

	}

}
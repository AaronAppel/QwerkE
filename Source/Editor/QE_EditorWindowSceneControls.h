#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowSceneControls : public EditorWindow
		{
		public:
			EditorWindowSceneControls::EditorWindowSceneControls(GUID guid = GUID()) :
				EditorWindow("Scene Controls", EditorWindowTypes::SceneControls, guid)
			{ }

		private:

			void DrawInternal() override;

            bool m_LastFramePaused = false;
		};

	}

}
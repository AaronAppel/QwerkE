#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDefaultDebug : public EditorWindow
		{
		public:
			EditorWindowDefaultDebug::EditorWindowDefaultDebug() :
				EditorWindow("Default Debug Window", EditorWindowFlags::ExactNameNoguid) { }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				ImGui::Text("Testing123");
			}
		};

	}

}
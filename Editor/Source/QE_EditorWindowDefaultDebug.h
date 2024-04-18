#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDefaultDebug : public EditorWindow
		{
		public:
			EditorWindowDefaultDebug::EditorWindowDefaultDebug(GUID guid) :
				EditorWindow("Default Debug Window", EditorWindowTypes::DefaultDebug, guid, EditorWindowFlags::ExactNameNoguid)
			{ }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				ImGui::Text("Testing123");
			}
		};

	}

}
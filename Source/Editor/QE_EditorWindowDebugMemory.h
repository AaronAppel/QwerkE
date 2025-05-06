#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugMemory : public EditorWindow
		{
		public:
			EditorWindowDebugMemory(GUID guid) :
				EditorWindow("Debug Memory",
					EditorWindowTypes::DebugMemory,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::ExactNameNoguid | EditorWindowFlags::Singleton))
			{ }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				ImGui::Text("TODO");
			}
		};

	}

}
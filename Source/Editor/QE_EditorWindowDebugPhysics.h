#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugPhysics : public EditorWindow
		{
		public:
			EditorWindowDebugPhysics(GUID guid) :
				EditorWindow("Debug Physics",
					EditorWindowTypes::DebugPhysics,
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
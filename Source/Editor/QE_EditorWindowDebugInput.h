#pragma once

#include "QF_Input.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugInput : public EditorWindow
		{
		public:
			EditorWindowDebugInput::EditorWindowDebugInput(GUID guid) :
				EditorWindow("Debug Input",
					EditorWindowTypes::DebugInput,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::ExactNameNoguid | EditorWindowFlags::Singleton))
			{ }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				Input::DrawDebugWindow();
			}
		};

	}

}
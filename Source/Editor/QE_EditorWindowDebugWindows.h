#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugWindows : public EditorWindow
		{
		public:
			EditorWindowDebugWindows(GUID guid) :
				EditorWindow("Debug Windows",
					EditorWindowTypes::DebugWindows,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::ExactNameNoguid | EditorWindowFlags::Singleton))
			{ }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
			}
		};

	}

}
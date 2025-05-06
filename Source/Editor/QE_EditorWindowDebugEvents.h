#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugEvents : public EditorWindow
		{
		public:
			EditorWindowDebugEvents(GUID guid) :
				EditorWindow("Debug Events",
					EditorWindowTypes::DebugEvents,
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
#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugProfiler : public EditorWindow
		{
		public:
			EditorWindowDebugProfiler(GUID guid) :
				EditorWindow("Debug Profiler",
					EditorWindowTypes::DebugProfiler,
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
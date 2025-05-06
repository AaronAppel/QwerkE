#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugRenderer : public EditorWindow
		{
		public:
			EditorWindowDebugRenderer(GUID guid) :
				EditorWindow("Debug Renderer",
					EditorWindowTypes::DebugRenderer,
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
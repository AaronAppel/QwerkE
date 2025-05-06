#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowInputMapping : public EditorWindow
		{
		public:
			EditorWindowInputMapping(GUID guid) :
				EditorWindow("Input Mapping",
					EditorWindowTypes::InputMapping,
					guid,
					(EditorWindowFlags)(0))
			{ }

		private:
			void DrawInternal() override
			{
			}
		};

	}

}
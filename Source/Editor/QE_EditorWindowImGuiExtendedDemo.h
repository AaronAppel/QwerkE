#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowImGuiExtendedDemo : public EditorWindow
		{
		public:
			EditorWindowImGuiExtendedDemo(GUID guid) :
				EditorWindow("ImGui Extended",
					EditorWindowTypes::ImGuiExtendedDemo,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::Singleton))
			{ }

		private:
			void DrawInternal() override;
		};

	}

}
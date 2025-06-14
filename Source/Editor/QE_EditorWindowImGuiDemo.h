#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowImGuiDemo : public EditorWindow
		{
		public:
			EditorWindowImGuiDemo(GUID guid) :
				EditorWindow("ImGui Demo",
					EditorWindowTypes::ImGuiDemo,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Hidden | EditorWindowFlags::Singleton))
			{ }

		private:
			void DrawInternal() override
			{
				bool isShowingDemo = true;
				ImGui::ShowDemoWindow(&isShowingDemo);

				if (!isShowingDemo)
				{
					ToggleHidden();
				}
			}
		};

	}

}
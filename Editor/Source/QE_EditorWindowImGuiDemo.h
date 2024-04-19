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
					(EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Hidden))
			{ }

		private:
			void DrawInternal() override
			{
				const auto result = m_WindowFlags;
				bool isShowingDemo = true;
				if (isShowingDemo)
				{
					ImGui::ShowDemoWindow(&isShowingDemo);
				}

				if (!isShowingDemo)
				{
					ToggleHidden();
				}
			}

			bool m_IsShowingImGuiDemoWindow = false;
		};

	}

}
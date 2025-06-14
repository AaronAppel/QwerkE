#pragma once

#include "Libraries/imgui-console/imgui_console.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowConsole : public EditorWindow
		{
		public:
			EditorWindowConsole(GUID guid);

		private:
			void DrawInternal() override
			{
				bool isShowingConsole = true;
				m_Console.Draw(&isShowingConsole);

				if (!isShowingConsole)
				{
					ToggleHidden();
				}
			}

			ImGuiConsole m_Console;
			ImVec4 m_ClearColor = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		};

	}

}
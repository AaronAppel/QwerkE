#pragma once

#include "Libraries/imgui-console/imgui_console.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowConsole : public EditorWindow
		{
		public:
			EditorWindowConsole(GUID guid) :
				EditorWindow("Console",
					EditorWindowTypes::Console,
					guid,
					// #TODO Avoid constructing already serialized data
					// m_WindowFlags)
					(EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Singleton))
			{ }

		private:
			void DrawInternal() override
			{
				bool isShowingConsole = m_WindowFlags ^ EditorWindowFlags::Hidden;
				if (isShowingConsole)
				{
					m_Console.Draw(&isShowingConsole);

					if (!isShowingConsole)
					{
						ToggleHidden();
					}
				}
			}

			ImGuiConsole m_Console;
		};

	}

}
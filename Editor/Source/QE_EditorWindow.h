#pragma once

#include "QC_Guid.h"

typedef int ImGuiWindowFlags;

namespace QwerkE {

	namespace Editor {

		class EditorWindow
		{
		public:
			void Draw();

		protected:
			EditorWindow(std::string windowName) : m_WindowName(windowName)
			{
				m_WindowName += "##";
				m_WindowName += std::to_string(m_Guid);
			}

			virtual void DrawInternal() = 0;

			ImGuiWindowFlags m_Flags = 0;

			const GUID& GetGuid() { return m_Guid; }

		private:
			std::string m_WindowName;
			GUID m_Guid;
			bool m_IsOpen = true;
		};

	}

}
#pragma once

#include "QC_Guid.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindow
		{
		public:
			EditorWindow(std::string windowName) : m_WindowName(windowName)
			{
				m_WindowName += "##";
				m_WindowName += std::to_string(m_Guid);
			}

			void Draw();

		protected:
			virtual void DrawInternal() { }

		private:
			std::string m_WindowName;
			GUID m_Guid;

		};

	}

}
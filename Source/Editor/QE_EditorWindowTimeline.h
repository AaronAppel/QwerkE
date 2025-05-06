#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Paths.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		// #TODO Consider re-naming ...DirectoryViewer
		class EditorWindowTimeline : public EditorWindow
		{
		public:
			EditorWindowTimeline(GUID guid = GUID()) :
				EditorWindow("Timeline", EditorWindowTypes::Timeline, guid)
			{ }

		private:
			void DrawInternal() override;
		};

	}

}
#pragma once

#include "QF_Enums.h"

namespace QwerkE {

	namespace Renderer {

		eOperationResult Initialize();
		void StartImGui();
		void EndImGui();
		void EndFrame();
		void Shutdown();

		void ToggleDebugStats();

	}

}
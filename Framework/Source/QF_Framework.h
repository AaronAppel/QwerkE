#pragma once

// #TODO Include paths for external/user use

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		eOperationResult Initialize();
		eOperationResult Shutdown();

		void NewFrame();
		void Update(float deltatime);
		void DrawImguiStart();
		void DrawImguiEnd();
		void DrawFrameEnd(u16 viewId = 0);

	}

}
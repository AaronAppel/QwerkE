#pragma once

#include "QF_Enums.h"

struct DebugDrawEncoder;

namespace QwerkE {

	namespace Renderer {

		// #TODO Think of having a viewport struct to manage rendering state for each view
		// struct ViewportState
		// {
		//	window*, viewId, width, height, frameBuffer
		// };

		eOperationResult Initialize();
		void Shutdown();

		void OnWindowResized(u32 newWidth, u32 newHeight);

#if _QDEARIMGUI // #TODO Move library specific code to some QF_Renderer_XXXX file
		void StartImGui();
		void EndImGui();
#endif // _QDEARIMGUI

		// void StartFrame(); // Just for library abstraction like StartImGui()
		void EndFrame();

		u16 NextViewId();

#ifdef QE_DEBUG
		void ToggleDebugStats();
		DebugDrawEncoder& DebugDrawer();
#endif

	}

}
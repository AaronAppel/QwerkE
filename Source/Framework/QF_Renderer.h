#pragma once

#include "QF_Enums.h"

struct DebugDrawEncoder;

namespace QwerkE {

	namespace Renderer {

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

#ifdef _QDEBUG
		void ToggleDebugStats();
		DebugDrawEncoder& DebugDrawer();
#endif

	}

}
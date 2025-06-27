#pragma once

#include "QF_Enums.h"

struct DebugDrawEncoder;

namespace QwerkE {

	namespace Renderer {

		eOperationResult Initialize();
		void Shutdown();

		void OnWindowResized(u32 newWidth, u32 newHeight);

#if _QDEARIMGUI
		void StartImGui();
		void EndImGui();
#endif // _QDEARIMGUI

		void EndFrame();

		u16 NextViewId();

#ifdef _QDEBUG
		void ToggleDebugStats();
		DebugDrawEncoder& DebugDrawer();
#endif

	}

}
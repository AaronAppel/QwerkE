#include "QF_Renderer.h"

#include <stdio.h> // #TODO Remove

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h> // bgfx::PlatformData
#include <bx/math.h> // #TODO Remove testing bgfx logo
#include <bgfx/../../src/bgfx_p.h> // bgfx::setGraphicsDebuggerPresent

#ifdef _QBGFXFRAMEWORK
#include "bgfxFramework/common.h"
#include "bgfxFramework/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "bgfxFramework/debugDraw/debugdraw.h"
#include "bgfxFramework/LoadShader.h"
#include "bgfxFramework/SampleRenderData.h"

#ifdef _QDEARIMGUI
#include "bgfxFramework/imguiCommon/imguiCommon.h"
#endif

#endif

#endif

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#include "Libraries/glfw/glfw3native.h"
#endif

#include "QF_Enums.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Renderer {

		bool s_showRendererDebugStats = false;

#ifdef _QBGFX
		const bgfx::ViewId s_ViewIdMain = 0;
#endif

		void OnWindowResize(int newWidth, int newHeight)
		{
#ifdef _QBGFX
			bgfx::reset((uint32_t)newWidth, (uint32_t)newHeight, BGFX_RESET_VSYNC);
			bgfx::setViewRect(s_ViewIdMain, 0, 0, bgfx::BackbufferRatio::Equal);
#endif
		}

		eOperationResult Initialize()
		{
#ifdef _QGLFW3
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());
#endif

#ifdef _QBGFX
			bgfx::renderFrame(); // Prevent bgfx from creating a separate render thread
			bgfx::Init init;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			init.platformData.ndt = glfwGetX11Display();
			init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
			init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
			init.platformData.nwh = glfwGetWin32Window(window);
#endif
			if (!bgfx::init(init))
				return eOperationResult::Failure;

			bgfx::setViewClear(s_ViewIdMain
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x303030ff
				, 1.0f
				, 0
			);
#endif
			const vec2& size = Window::GetResolution();
			OnWindowResize(size.x, size.y);

#ifdef _QDebug
			bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiCreate();
			ddInit();
#endif
#endif
			return eOperationResult::Success;
		}

		void StartImGui()
		{
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			const vec2& windowSize = Window::GetResolution();

			imguiBeginFrame(
				x, y
				, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1 ? IMGUI_MBUT_LEFT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1 ? IMGUI_MBUT_RIGHT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1 ? IMGUI_MBUT_MIDDLE : 0
				, 0
				, uint16_t(windowSize.x)
				, uint16_t(windowSize.y)
			);
		}

		void EndImGui()
		{
			imguiEndFrame();
		}

		void EndFrame()
		{
#ifdef _QBGFX
			bgfx::frame();
#ifdef _QDebug
			bgfx::setDebug(s_showRendererDebugStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
			bgfx::dbgTextClear();
#endif
#endif
		}

		void Shutdown()
		{
#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiDestroy();
#endif

			ddShutdown();

			bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
			bgfx::shutdown();
#endif
		}

		void ToggleDebugStats()
		{
			s_showRendererDebugStats = !s_showRendererDebugStats;
		}

	}

}
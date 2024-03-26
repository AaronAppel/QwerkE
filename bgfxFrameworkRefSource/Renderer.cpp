#include "Renderer.h"

#include <stdio.h>

#ifdef _QDEARIMGUI
#include "include/bgfxFramework/imguiCommon/imguiCommon.h"
#endif

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h> // bgfx::PlatformData
#include <bgfx/../../src/bgfx_p.h> // bgfx::setGraphicsDebuggerPresent

#ifdef _QBGFXFRAMEWORK
#include "bgfxFramework/common.h"
#include "bgfxFramework/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "bgfxFramework/debugDraw/debugdraw.h"
#include "bgfxFramework/LoadShader.h"
#endif

#endif

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#include "Libraries/glfw/glfw3native.h"
#endif

#include "SampleRenderData.h"
#include "Window.h"

namespace Renderer {

	bool s_showStats = false;
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

	bool Initialize()
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
			return false;

		bgfx::setViewClear(s_ViewIdMain
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);
#endif

		int width, height;
		Window::GetWindowSize(&width, &height);
		OnWindowResize(width, height);

#ifdef _QDebug
		bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
		imguiCreate();
		ddInit();
#endif
#endif

		return true;
	}

	void EndFrame()
	{
#ifdef _QDebug
		bgfx::frame();
		bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT); // Enable stats or debug text
		bgfx::dbgTextClear();
#endif
	}

	void Shutdown()
	{
#ifdef _QDEARIMGUI
		imguiDestroy();
#endif

#ifdef _QBGFX
		ddShutdown();

		bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
		bgfx::shutdown();
#endif
	}

	void ToggleDebugStats()
	{
		s_showStats = !s_showStats;
	}

}

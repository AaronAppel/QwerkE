#include "QF_Renderer.h"

#include <stdio.h> // #TODO Remove

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h> // bgfx::PlatformData
#include <bgfx/../../src/bgfx_p.h> // bgfx::setGraphicsDebuggerPresent
#include <bx/math.h> // #TODO Remove testing bgfx logo
#include <bgfx/logo.h> // #TODO Remove testing bgfx logo
#include <bx/rng.h> // #TODO Picking

#ifndef BX_CONFIG_DEBUG // From bx/config.h
#	error "BX_CONFIG_DEBUG must be defined in build script!"
#endif // BX_CONFIG_DEBUG

#ifdef _QBGFXFRAMEWORK
#include "bgfxFramework/common.h"
#include "bgfxFramework/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "bgfxFramework/debugDraw/debugdraw.h"
#include "bgfxFramework/LoadShader.h"
#include <bgfxFramework/SampleRenderData.h>

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

#include "QF_Assets.h"
#include "QF_Enums.h"
#include "QF_Paths.h"
#include "QF_RendererHelpers.h"
#include "QF_Shader.h"
#include "QF_Texture.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Renderer {

		static bool s_showRendererDebugStats = false;

#ifdef _QBGFX // #TODO Move library specific code to some QF_Renderer_XXXX file

		// #TODO Look at using 0 or back buffer to catch poor renderer state, rendering to back buffer (possibly after bgfx::reset)
		// static const bgfx::ViewId s_ViewIdBackBuffer = 0;

		static const bgfx::ViewId s_ViewIdMain = 0;
		static const bgfx::ViewId s_ViewIdImGui = 1;
		static bgfx::ViewId s_NextViewId = s_ViewIdImGui + 1;
		static u16 s_ViewIdMax = 100;
#ifdef _QDEBUG
		static DebugDrawEncoder* s_DebugDrawer = nullptr;
#endif
#endif

		void OnWindowResized(u32 newWidth, u32 newHeight)
		{
			// assert(newWidth > 0 && newHeight > 0);
			if (1 > newWidth || 1 > newHeight)
			{
				return;
				// #TODO Potentially invalid resolution.
				// May need to enforce a minimum, but at least handle resetting/updating differently
			}

#ifdef _QBGFX
			// u32 featurestateFlags = vsyncEnabled(BGFX_RESET_VSYNC) | ToBitFlag(multiSamplingAntiAliasingLevel(BGFX_RESET_MSAA_X0/4/8/16/etc));
			bgfx::reset(newWidth, newHeight, BGFX_RESET_VSYNC); // BGFX_RESET_NONE, BGFX_RESET_MSAA_X16
			// bgfx::setViewRect(s_ViewIdMain, 0, 0, bgfx::BackbufferRatio::Equal);

			// bgfx::setViewRect(s_ViewIdMain, 0, 0, newWidth, newHeight);
			// bgfx::setViewRect(s_ViewIdImGui, 0, 0, newWidth, newHeight);

			for (size_t i = 0; i < s_NextViewId - 1; i++)
			{
				bgfx::setViewRect(i, 0, 0, newWidth, newHeight);
				bgfx::setViewFrameBuffer(i, BGFX_INVALID_HANDLE);

				// DEBUG Change clear color to red to better catch writing to the back buffer
				// bgfx::setViewClear(i,
				// 	BGFX_CLEAR_COLOR,
				// 	0xff0000ff, 1.0f, 0);
			}
#endif
		}

		eOperationResult Initialize()
		{
#ifdef _QGLFW3
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());
#endif

#ifdef _QBGFX
			// #TODO Crash here on (re)loading engine a 2nd time
			bgfx::renderFrame(); // #NOTE Prevent bgfx from creating a separate render thread
			bgfx::Init init;

			PosColorVertex::init(); // Create vertex stream declaration.

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			init.platformData.ndt = glfwGetX11Display();
			init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
			init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
			init.platformData.nwh = glfwGetWin32Window(window);
			// init.type = bgfx::RendererType::Direct3D11;
#endif
			const vec2f& windowSize = Window::GetSize();
			init.resolution.width = windowSize.x;
			init.resolution.height = windowSize.y;
			init.resolution.reset = BGFX_RESET_VSYNC;

			if (!bgfx::init(init))
				return eOperationResult::Failure;

			bgfx::setViewClear(s_ViewIdMain
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x303030ff
				, 1.0f
				, 0
			);

			bgfx::setViewClear(s_ViewIdImGui
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x303030ff
				, 1.0f
				, 0
			);
#endif
			const vec2f& size = Window::GetSize();
			OnWindowResized(size.x, size.y);

#ifdef _QDEBUG
			bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiCreate(18.f);
#ifdef _QDEBUG
			ddInit();
			s_DebugDrawer = new DebugDrawEncoder();
#endif
#endif
			bgfx::touch(s_ViewIdMain); // Render main view 1st frame
#endif

			return eOperationResult::Success;
		}

		void Shutdown()
		{
			if (Window::IsMinimized())
				return; // #TODO Review shutdown while minimized

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiDestroy();
#endif
			ddShutdown();

			bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
			bgfx::shutdown();
#endif

#ifdef _QDEBUG
			delete s_DebugDrawer;
#endif
		}

#if _QDEARIMGUI
		void StartImGui()
		{
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			const vec2f& windowSize = Window::GetSize();

			// #TODO Review StartFrame() logic IF main view needs constant updating
			// bgfx::setViewRect(s_ViewIdMain, 0, 0, windowSize.x, windowSize.y);

			imguiBeginFrame(
				x, y
				, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1 ? IMGUI_MBUT_LEFT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1 ? IMGUI_MBUT_RIGHT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1 ? IMGUI_MBUT_MIDDLE : 0
				, 0
				, u16(windowSize.x)
				, u16(windowSize.y)
				, -1
				, s_ViewIdImGui
			);

			bgfx::touch(s_ViewIdImGui);
		}

		void EndImGui()
		{
			imguiEndFrame();
		}
#endif // _QDEARIMGUI

		void EndFrame()
		{
			if (Window::IsMinimized())
				return;

#ifdef _QBGFX
			bgfx::frame();

#ifdef _QDEBUG
			bgfx::setDebug(s_showRendererDebugStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
			bgfx::dbgTextClear();
#endif
#endif
		}

		u16 NextViewId()
		{
			ASSERT(s_NextViewId < s_ViewIdMax, "ViewId limit reached!");
			return s_NextViewId++;
		}

#ifdef _QDEBUG
		void ToggleDebugStats()
		{
			s_showRendererDebugStats = !s_showRendererDebugStats;
		}

		DebugDrawEncoder& DebugDrawer()
		{
			ASSERT(s_DebugDrawer, "Debug Drawer is null!");
			return *s_DebugDrawer;
		}
#endif

	}

}
#include "QF_Renderer.h"

#include <stdio.h> // #TODO Remove

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h> // bgfx::PlatformData
#include <bgfx/../../src/bgfx_p.h> // bgfx::setGraphicsDebuggerPresent
#include <bx/math.h> // #TODO Remove testing bgfx logo
#include <bgfx/logo.h> // #TODO Remove testing bgfx logo

#ifndef BX_CONFIG_DEBUG // From bx/config.h
#	error "BX_CONFIG_DEBUG must be defined in build script!"
#endif // BX_CONFIG_DEBUG

#ifdef _QBGFXFRAMEWORK
#include "bgfxFramework/common.h"
#include "bgfxFramework/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "bgfxFramework/debugDraw/debugdraw.h"
#include "bgfxFramework/LoadShader.h"

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

		static bool s_showRendererDebugStats = false;

#ifdef _QBGFX
		static const bgfx::ViewId s_ViewIdMain = 0;
		static const bgfx::ViewId s_ViewIdImGui = 1;
		static const bgfx::ViewId s_ViewIdFbo1 = 2;

		// #TODO static these
		bgfx::FrameBufferHandle s_FrameBufferHandle; // #TESTING
		const u8 s_FrameBufferTextureCount = 1;
		bgfx::TextureHandle s_FrameBufferTextures[s_FrameBufferTextureCount]; // #TODO Destroy
		bgfx::TextureHandle s_ReadBackTexture; // #TODO Destroy

#ifdef _QDEBUG
		static DebugDrawEncoder* s_DebugDrawer = nullptr;
#endif
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
			const vec2& size = Window::GetSize();
			OnWindowResize(size.x, size.y);

#ifdef _QDEBUG
			bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiCreate(18.f);
			ddInit();
#endif
			// the regular image texture
			const vec2& windowSize = Window::GetSize();
			const bool has_mips = false;
			const uint16_t num_layers = 1;

			s_ReadBackTexture = bgfx::createTexture2D(windowSize.x, windowSize.y,
				has_mips, num_layers,
				bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);
			ASSERT(bgfx::isValid(s_ReadBackTexture), "Error creating read back texture!");

			s_FrameBufferTextures[0] = bgfx::createTexture2D(windowSize.x, windowSize.y,
				has_mips, num_layers,
				bgfx::TextureFormat::BGRA8,
				BGFX_TEXTURE_RT);
			ASSERT(bgfx::isValid(s_FrameBufferTextures[0]), "Error creating frame buffer texture [0]!");

			s_FrameBufferHandle = bgfx::createFrameBuffer(s_FrameBufferTextureCount, s_FrameBufferTextures); // #TESTING
			ASSERT(bgfx::kInvalidHandle != s_FrameBufferHandle.idx, "Error creating frame buffer!");

			bgfx::touch(s_ViewIdMain); // Render main view 1st frame
#endif

#ifdef _QDEBUG
			s_DebugDrawer = new DebugDrawEncoder();
#endif

			return eOperationResult::Success;
		}

		void StartImGui()
		{
			if (Window::IsMinimized())
				return;

			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			const vec2& windowSize = Window::GetSize();

			imguiBeginFrame(
				x, y
				, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1 ? IMGUI_MBUT_LEFT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1 ? IMGUI_MBUT_RIGHT : 0
				| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1 ? IMGUI_MBUT_MIDDLE : 0
				, 0
				, uint16_t(windowSize.x)
				, uint16_t(windowSize.y)
				, -1
				, s_ViewIdImGui
			);

			bgfx::touch(s_ViewIdImGui); // #REVIEW
		}

		void EndImGui()
		{
			if (Window::IsMinimized())
				return;

			imguiEndFrame();
		}

		void EndFrame()
		{
			if (Window::IsMinimized())
				return;

#ifdef _QBGFX
			bgfx::dbgTextPrintf(0, 3, 0x0f, "Framebuffer %i", (int)s_ViewIdFbo1);

			// bgfx::setViewFrameBuffer(s_ViewIdMain, frame_buffer_handle_);

			bgfx::frame();
			bgfx::touch(s_ViewIdMain);
#ifdef _QDEBUG
			bgfx::setDebug(s_showRendererDebugStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
			bgfx::dbgTextClear();
#endif
#endif
		}

		void Shutdown()
		{
			if (Window::IsMinimized())
				return;

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiDestroy();
#endif
			bgfx::destroy(s_FrameBufferHandle); // #TESTING

			ddShutdown();

			bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
			bgfx::shutdown();
#endif

#ifdef _QDEBUG
			delete s_DebugDrawer;
#endif

		}

		void ToggleDebugStats()
		{
			s_showRendererDebugStats = !s_showRendererDebugStats;
		}

		DebugDrawEncoder& DebugDrawer()
		{
			return *s_DebugDrawer;
		}

	}

}
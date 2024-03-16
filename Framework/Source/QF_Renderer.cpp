#include "QF_Renderer.h"

#include <stdio.h>

#ifdef _QBGFX
// #ifdef _QDEARIMGUI
// #TODO Fix bigg and bgfx imgui functionality
// #include "Libraries/bgfxImgui/bgfxImgui/imgui.h"
// #include "Libraries/bigg/bigg_imgui.hpp"
// #include "Libraries/imgui/imgui.h"
// #include "Libraries/bgfx/3rdparty/dear-imgui/imgui_user.h"
// #endif
#include "Libraries/bgfx/examples/common/common.h"
#include "Libraries/bgfx/examples/common/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#include "Libraries/bgfx/include/bgfx/platform.h" // bgfx::PlatformData
#include "Libraries/bgfx/src/bgfx_p.h" // bgfx::setGraphicsDebuggerPresent
#include "Libraries/bgfx/logo.h" // #TODO Remove testing bgfx logo
#include "Libraries/bx/include/bx/math.h" // #TODO Remove testing bgfx logo
#endif

#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtx/euler_angles.hpp"
#endif

#include "QC_Time.h"

#include "QF_Window.h"

#ifdef _QBGFX
#include "QF_RendererTempData.h"
#endif

namespace QwerkE {

	namespace Renderer {

		bool s_showStats = false;
		float s_ElapsedTime = 0.f; // #TODO Remove test code

#ifdef _QBGFX
		const bgfx::ViewId s_ViewIdMain = 0;

		bgfx::VertexBufferHandle mVbh; // #TODO Remove test code
		bgfx::IndexBufferHandle mIbh;

		Mesh* m_mesh;

		bgfx::ProgramHandle mProgram;
#endif

// #ifdef _QDEARIMGUI
		// ImGuiContext* m_imgui = nullptr;
		// bx::AllocatorI* m_allocator;
		// bgfx::VertexLayout  m_layout;
		// bgfx::ProgramHandle m_program;
		// bgfx::ProgramHandle m_imageProgram;
		// bgfx::TextureHandle m_texture;
		// bgfx::UniformHandle s_tex;
		// bgfx::UniformHandle u_imageLodEnabled;
		// ImFont* m_font[ImGui::Font::Count];
		// int64_t m_last;
		// int32_t m_lastScroll;
		// bgfx::ViewId m_viewId;
// #endif

		void OnWindowResize(int newWidth, int newHeight)
		{
			bgfx::reset((uint32_t)newWidth, (uint32_t)newHeight, BGFX_RESET_VSYNC);
			bgfx::setViewRect(s_ViewIdMain, 0, 0, bgfx::BackbufferRatio::Equal);
		}

		static void OnKeyEvent(int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
				s_showStats = !s_showStats;
		}

		eOperationResult Initialize()
		{
#ifdef _QGLFW3
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());
#endif
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
			const vec2& size = Window::GetResolution();
			OnWindowResize(size.x, size.y);

			/*
#ifdef _QDEARIMGUI
			imguiInit(mWindow);
#endif

			uint32_t flags = 0;
			bgfx::reset(mWidth, mHeight, flags);

#ifdef _QDEARIMGUI
			imguiReset(mWidth, mHeight);
#endif
			*/

			PosColorVertex::init();

			// #TODO Remove test code
			mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
			mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

			bgfx::setDebug(BGFX_DEBUG_TEXT);

			// bgfx::ShaderHandle vsh = myLoadShader("vs_mesh.bin");
			// bgfx::ShaderHandle fsh = myLoadShader("fs_mesh.bin");
			bgfx::ShaderHandle vsh = myLoadShader("vs_cubes.bin");
			bgfx::ShaderHandle fsh = myLoadShader("fs_cubes.bin");
			mProgram = bgfx::createProgram(vsh, fsh, true);

			// Create program from shaders.
			// mProgram = loadProgram("vs_mesh", "fs_mesh");

			// m_mesh = meshLoad("meshes/bunny.bin");

#ifdef _QDebug
			// Input::OnKeyCallback(OnKeyEvent)
#endif

// #ifdef _QDEARIMGUI
			// imguiCreate();

			// m_imgui = ImGui::CreateContext();
			// ImGuiIO& io = ImGui::GetIO();
// #endif

			return eOperationResult::Success;
		}

		void EndFrame()
		{
			const vec2& size = Window::GetResolution();
			bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(0), 20) - 20, bx::max<uint16_t>(uint16_t(size.y / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
			bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
			bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
			bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
			bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
			const bgfx::Stats* stats = bgfx::getStats();
			bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

#ifdef _QDEARIMGUI
			// #TODO Fix bigg and bgfx imgui functionality
			// imguiEvents(dt);
			// ImGui::NewFrame();
			// ImGui::Render();
			// imguiRender(ImGui::GetDrawData());
#endif

#ifdef _QGLM
			s_ElapsedTime += Time::FrameDelta();

			const glm::vec3 at = { 0.f, 0.f,   0.f };
			const glm::vec3 eye = { 0.f, 0.f, -35.f };
			const glm::vec3 up = { 0.f, 1.f, 0.f };

			glm::mat4 view = glm::lookAt(eye, at, up);
			glm::mat4 proj = glm::perspective(glm::radians(60.0f), size.x / size.y, .1f, 100.f);
#ifdef _QBGFX
			bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);

			int32_t m_pt = 0;

			bool m_r = true;
			bool m_g = true;
			bool m_b = true;
			bool m_a = true;

			uint64_t state = 0
				| (m_r ? BGFX_STATE_WRITE_R : 0)
				| (m_g ? BGFX_STATE_WRITE_G : 0)
				| (m_b ? BGFX_STATE_WRITE_B : 0)
				| (m_a ? BGFX_STATE_WRITE_A : 0)
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CW
				| BGFX_STATE_MSAA
				| s_ptState[m_pt]
				;

			for (uint32_t yy = 0; yy < 11; ++yy)
			{
				for (uint32_t xx = 0; xx < 11; ++xx)
				{
					glm::mat4 mtx = glm::mat4(1.f);
					mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 0.0f));
					mtx *= glm::yawPitchRoll(s_ElapsedTime + xx * 0.21f, s_ElapsedTime + yy * 0.37f, 0.0f);
					bgfx::setTransform(&mtx[0][0]);
					bgfx::setVertexBuffer(0, mVbh);
					bgfx::setIndexBuffer(mIbh);
					bgfx::setState(state);
					bgfx::submit(s_ViewIdMain, mProgram);
				}
			}

			bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT); // Enable stats or debug text
			bgfx::frame();
			bgfx::touch(s_ViewIdMain); // View s_ViewIdMain is cleared if no other draw calls are submitted to view s_ViewIdMain
			bgfx::dbgTextClear();
#endif
#endif
		}

		void Shutdown()
		{
// #ifdef _QDEARIMGUI
			// imguiDestroy();
			// ImGui::DestroyContext();
// #endif

#ifdef _QBGFX
			bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
			bgfx::shutdown();
#endif
		}

	}

}
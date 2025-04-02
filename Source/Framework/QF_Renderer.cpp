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
#include "QF_Window.h"

namespace QwerkE {

	namespace Renderer {

		static bool s_showRendererDebugStats = false;

#ifdef _QBGFX
		// Object picking
// #define RENDER_PASS_SHADING 0  // Default forward rendered geo with simple shading
// #define RENDER_PASS_ID      1  // ID buffer for picking
// #define RENDER_PASS_BLIT    2  // Blit GPU render target to CPU texture

#define ID_DIM 8  // Size of the ID buffer

		bgfxFramework::Mesh* m_meshes[12];
		float m_meshScale[12];
		float m_idsF[12][4];
		uint32_t m_idsU[12];
		uint32_t m_highlighted;

		// Resource handles
		Shader* m_shadingProgram = nullptr;
		Shader* m_idProgram = nullptr;
		bgfx::UniformHandle u_tint;
		bgfx::UniformHandle u_id;
		bgfx::TextureHandle m_pickingRT;
		bgfx::TextureHandle m_pickingRTDepth;
		bgfx::TextureHandle m_blitTex;
		bgfx::FrameBufferHandle m_pickingFB;

		uint8_t m_blitData[ID_DIM * ID_DIM * 4]; // Read blit into this

		uint32_t m_reading;
		uint32_t m_currFrame;
		static const bgfx::ViewId s_ViewIdPicking = 3;
		//
		static const bgfx::ViewId s_ViewIdMain = 0;
		static const bgfx::ViewId s_ViewIdImGui = 1;
		static const bgfx::ViewId s_ViewIdFbo1 = 2;

		static bgfx::FrameBufferHandle s_FrameBufferHandleFbo; // #TESTING
		static bgfx::TextureHandle s_FrameBufferTexturesFBO1[2];
		static bgfx::FrameBufferHandle s_FrameBufferHandleEditorCamera; // #TESTING
		static bgfx::TextureHandle s_FrameBufferTextureEditorCamera;

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
			OnWindowResize(size.x, size.y);

#ifdef _QDEBUG
			bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiCreate(18.f);
#ifdef _QDEBUG
			ddInit();
#endif
#endif
			const bool has_mips = false;
			const uint16_t num_layers = 1;

			// s_ReadBackTexture = bgfx::createTexture2D(windowSize.x, windowSize.y,
			// 	has_mips, num_layers,
			// 	bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);
			// ASSERT(bgfx::isValid(s_ReadBackTexture), "Error creating read back texture!");

			{
				s_FrameBufferTexturesFBO1[0] = bgfx::createTexture2D(windowSize.x, windowSize.y,
					has_mips, num_layers,
					bgfx::TextureFormat::BGRA8,
					BGFX_TEXTURE_RT);
				ASSERT(bgfx::isValid(s_FrameBufferTexturesFBO1[0]), "Error creating frame buffer texture [0]!");
			}

			{
				s_FrameBufferTexturesFBO1[1] = bgfx::createTexture2D(windowSize.x, windowSize.y,
					has_mips, num_layers,
					bgfx::TextureFormat::D16,
					BGFX_TEXTURE_RT_WRITE_ONLY);
				ASSERT(bgfx::isValid(s_FrameBufferTexturesFBO1[1]), "Error creating frame buffer depth texture [1]!");

				s_FrameBufferHandleFbo = bgfx::createFrameBuffer(2, s_FrameBufferTexturesFBO1); // #TESTING
				ASSERT(bgfx::kInvalidHandle != s_FrameBufferHandleFbo.idx, "Error creating frame buffer!");
			}

			{	// SetupFBO view
				bgfx::setViewName(s_ViewIdFbo1, "FBO1");
				bgfx::setViewFrameBuffer(s_ViewIdFbo1, s_FrameBufferHandleFbo);
				bgfx::setViewClear(s_ViewIdFbo1
					, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
					, 0x303030ff
					, 1.0f
					, 0
				);
			}

			{
				s_FrameBufferTextureEditorCamera = bgfx::createTexture2D(windowSize.x, windowSize.y,
					has_mips, num_layers,
					bgfx::TextureFormat::BGRA8,
					BGFX_TEXTURE_RT);
				ASSERT(bgfx::isValid(s_FrameBufferTextureEditorCamera), "Error creating frame buffer texture [0]!");

				s_FrameBufferHandleEditorCamera = bgfx::createFrameBuffer(1, &s_FrameBufferTextureEditorCamera); // #TESTING
				ASSERT(bgfx::kInvalidHandle != s_FrameBufferHandleEditorCamera.idx, "Error creating frame buffer!");
			}

			{	// Setup editor camera view
				bgfx::setViewName(s_ViewIdMain, "Main");
				bgfx::setViewFrameBuffer(s_ViewIdMain, s_FrameBufferHandleEditorCamera);
				bgfx::setViewClear(s_ViewIdMain
					, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
					, 0x303030ff
					, 1.0f
					, 0
				);
			}

			{	// Object picking
				bgfx::setViewName(s_ViewIdPicking, "Picking");
				bgfx::setViewFrameBuffer(s_ViewIdPicking, m_pickingFB);
				bgfx::setViewClear(s_ViewIdPicking
					, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
					, 0x000000ff
					, 1.0f
					, 0
				);

				// Create uniforms
				u_tint = bgfx::createUniform("u_tint", bgfx::UniformType::Vec4); // Tint for when you click on items
				u_id = bgfx::createUniform("u_id", bgfx::UniformType::Vec4); // ID for drawing into ID buffer

				// Create program from shaders.
				m_shadingProgram = new Shader();
				// #TODO Consider moving below shader creation code to utility or renderer
				m_shadingProgram->m_Program = myLoadShaderProgram( // Blinn shading
					Paths::Shader("vs_picking_shaded.bin").c_str(),
					Paths::Shader("fs_picking_shaded.bin").c_str()
				);

				m_idProgram = new Shader();
				m_idProgram->m_Program = myLoadShaderProgram( // Shader for drawing into ID buffer
					Paths::Shader("vs_picking_shaded.bin").c_str(),
					Paths::Shader("fs_picking_id.bin").c_str()
				);

				// #TODO Call Assets to find and create shaders (Assets calling utility load function)
				// m_shadingProgram = Assets::Get<Shader>( );
				// m_idProgram = Assets::Get<Shader>( );

				m_highlighted = UINT32_MAX;
				m_reading = 0;
				m_currFrame = UINT32_MAX;

				bx::RngMwc mwc;  // Random number generator
				for (uint32_t ii = 0; ii < 12; ++ii)
				{
					// #TODO See how m_meshes is used in bgfx example
					// m_meshes[ii] = meshLoad(meshPaths[ii % BX_COUNTOF(meshPaths)]);
					// m_meshScale[ii] = meshScale[ii % BX_COUNTOF(meshPaths)];

					// For the sake of this example, we'll give each mesh a random color,  so the debug output looks colorful.
					// In an actual app, you'd probably just want to count starting from 1
					uint32_t rr = mwc.gen() % 256;
					uint32_t gg = mwc.gen() % 256;
					uint32_t bb = mwc.gen() % 256;
					m_idsF[ii][0] = rr / 255.0f;
					m_idsF[ii][1] = gg / 255.0f;
					m_idsF[ii][2] = bb / 255.0f;
					m_idsF[ii][3] = 1.0f;
					m_idsU[ii] = rr + (gg << 8) + (bb << 16) + (255u << 24);
				}

				// Set up ID buffer, which has a color target and depth buffer
				m_pickingRT = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
					| BGFX_TEXTURE_RT
					| BGFX_SAMPLER_MIN_POINT
					| BGFX_SAMPLER_MAG_POINT
					| BGFX_SAMPLER_MIP_POINT
					| BGFX_SAMPLER_U_CLAMP
					| BGFX_SAMPLER_V_CLAMP
				);
				m_pickingRTDepth = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::D32F, 0
					| BGFX_TEXTURE_RT
					| BGFX_SAMPLER_MIN_POINT
					| BGFX_SAMPLER_MAG_POINT
					| BGFX_SAMPLER_MIP_POINT
					| BGFX_SAMPLER_U_CLAMP
					| BGFX_SAMPLER_V_CLAMP
				);

				// CPU texture for blitting to and reading ID buffer so we can see what was clicked on.
				// Impossible to read directly from a render target, you *must* blit to a CPU texture
				// first. Algorithm Overview: Render on GPU -> Blit to CPU texture -> Read from CPU
				// texture.
				m_blitTex = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
					| BGFX_TEXTURE_BLIT_DST
					| BGFX_TEXTURE_READ_BACK
					| BGFX_SAMPLER_MIN_POINT
					| BGFX_SAMPLER_MAG_POINT
					| BGFX_SAMPLER_MIP_POINT
					| BGFX_SAMPLER_U_CLAMP
					| BGFX_SAMPLER_V_CLAMP
				);

				bgfx::TextureHandle rt[2] =
				{
					m_pickingRT,
					m_pickingRTDepth
				};
				m_pickingFB = bgfx::createFrameBuffer(BX_COUNTOF(rt), rt, true);
			}

			bgfx::touch(s_ViewIdMain); // Render main view 1st frame
#endif

#ifdef _QDEBUG
			s_DebugDrawer = new DebugDrawEncoder();
#endif

			return eOperationResult::Success;
		}

		void StartImGui()
		{
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			const vec2f& windowSize = Window::GetSize();

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
			imguiEndFrame();
		}

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

		void Shutdown()
		{
			if (Window::IsMinimized())
				return;

#ifdef _QBGFX
#ifdef _QDEARIMGUI
			imguiDestroy();
#endif
			bgfx::destroy(s_FrameBufferHandleFbo);
			bgfx::destroy(s_FrameBufferHandleEditorCamera);

			bgfx::destroy(s_FrameBufferTexturesFBO1[0]);
			bgfx::destroy(s_FrameBufferTexturesFBO1[1]);
			bgfx::destroy(s_FrameBufferTextureEditorCamera);

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
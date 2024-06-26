#include "QF_Renderer.h"

#include <stdio.h>

#ifdef _QDEARIMGUI
// #TODO Fix bigg and bgfx imgui functionality
// #include "common/imgui/imgui.h"
// #include "Libraries/bigg/bigg_imgui.hpp"
#endif

#ifdef _QBGFX
#include "Libraries/bgfx/examples/common/common.h"
#include "Libraries/bgfx/examples/common/bgfx_utils.h" // Requires "..\Libraries\bimg\include"
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#include "Libraries/bgfx/include/bgfx/platform.h"
#include "Libraries/bgfx/logo.h"
#include <bx/thread.h>
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
bgfx::ShaderHandle myLoadShader(const char* FILENAME)
{
	const char* shaderPath = "???";

	switch (bgfx::getRendererType()) {
		case bgfx::RendererType::Noop:
		// case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
		case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
		case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
		case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char* filePath = (char*)malloc(shaderLen + fileLen + 1);

	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);
	filePath[shaderLen + fileLen] = '\0';

	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	}
	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};
static const uint16_t s_cubeTriList[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };
#endif

static bx::DefaultAllocator s_allocator;
static bx::SpScUnboundedQueue s_apiThreadEvents(&s_allocator);

enum class EventType
{
	Exit,
	Key,
	Resize
};

struct ExitEvent
{
	EventType type = EventType::Exit;
};

struct KeyEvent
{
	EventType type = EventType::Key;
	int key;
	int action;
};

struct ResizeEvent
{
	EventType type = EventType::Resize;
	uint32_t width;
	uint32_t height;
};

struct ApiThreadArgs
{
	bgfx::PlatformData platformData;
	uint32_t width;
	uint32_t height;
};

static int32_t runApiThread(bx::Thread* self, void* userData)
{
	auto args = (ApiThreadArgs*)userData;
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
	init.platformData = args->platformData;
	init.resolution.width = args->width;
	init.resolution.height = args->height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	// init.type = bgfx::RendererType::Direct3D11;
	if (!bgfx::init(init))
		return 1;
	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
	uint32_t width = args->width;
	uint32_t height = args->height;
	bool showStats = false;
	bool exit = false;
	while (!exit) {
		// Handle events from the main thread.
		while (auto ev = (EventType*)s_apiThreadEvents.pop()) {
			if (*ev == EventType::Key) {
				auto keyEvent = (KeyEvent*)ev;
				if (keyEvent->key == GLFW_KEY_F1 && keyEvent->action == GLFW_RELEASE)
					showStats = !showStats;
			}
			else if (*ev == EventType::Resize) {
				auto resizeEvent = (ResizeEvent*)ev;
				bgfx::reset(resizeEvent->width, resizeEvent->height, BGFX_RESET_VSYNC);
				bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
				width = resizeEvent->width;
				height = resizeEvent->height;
			}
			else if (*ev == EventType::Exit) {
				exit = true;
			}
			delete ev;
		}
		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(kClearView);
		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
		// Enable stats or debug text.
		bgfx::setDebug(showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		// Advance to next frame. Main thread will be kicked to process submitted rendering primitives.
	}
	bgfx::shutdown();
	return 0;
}

namespace QwerkE {

	namespace Renderer {

#ifdef _QGLFW3
		GLFWwindow* mWindow = nullptr;
#endif
		uint32_t mWidth = 1280;
		uint32_t mHeight = 720;
		uint32_t mReset = 0;

		float s_ElapsedTime = 0.f;

#ifdef _QBGFX
		bx::Thread s_ApiThread;

		bgfx::VertexBufferHandle mVbh;
		bgfx::IndexBufferHandle mIbh;

		bgfx::ShaderHandle vsh;
		bgfx::ShaderHandle fsh;
		bgfx::ProgramHandle mProgram;
#endif

		eOperationResult Initialize()
		{
#ifdef _QGLFW3
			mWindow = static_cast<GLFWwindow*>(Window::GetContext());
#endif
			// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
			// Most graphics APIs must be used on the same thread that created the window.
			// bgfx::renderFrame();
			// Create a thread to call the bgfx API from (except bgfx::renderFrame).
			ApiThreadArgs apiThreadArgs;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			apiThreadArgs.platformData.ndt = glfwGetX11Display();
			apiThreadArgs.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
			apiThreadArgs.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
			apiThreadArgs.platformData.nwh = glfwGetWin32Window(mWindow);
#endif
			int width, height;
			glfwGetWindowSize(mWindow, &width, &height);
			apiThreadArgs.width = (uint32_t)width;
			apiThreadArgs.height = (uint32_t)height;
			s_ApiThread.init(runApiThread, &apiThreadArgs);

			/*
#ifdef _QBGFX
			bgfx::PlatformData platformData;
			memset(&platformData, 0, sizeof(platformData));
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(mWindow);
			platformData.ndt = glfwGetX11Display();
#elif BX_PLATFORM_OSX
			platformData.nwh = glfwGetCocoaWindow(mWindow);
#elif BX_PLATFORM_WINDOWS
			platformData.nwh = glfwGetWin32Window(mWindow);
#endif // BX_PLATFORM_
			bgfx::setPlatformData(platformData);

			// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
			// Most graphics APIs must be used on the same thread that created the window.
			bgfx::renderFrame();

			bgfx::Init init;
			init.platformData = platformData;
			init.type = bgfx::RendererType::Count;
			init.vendorId = BGFX_PCI_ID_NONE;
			init.deviceId = 0;
			init.callback = NULL;
			init.allocator = NULL;
			bgfx::init(init);
#ifdef _QDEARIMGUI
			imguiInit(mWindow);
#endif

			uint32_t flags = 0;
			bgfx::reset(mWidth, mHeight, flags);

#ifdef _QDEARIMGUI
			imguiReset(mWidth, mHeight);
#endif

			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
			bgfx::setViewRect(0, 0, 0, mWidth, mHeight);

			PosColorVertex::init();
			mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
			mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

			bgfx::setDebug(BGFX_DEBUG_TEXT);

			vsh = myLoadShader("vs_cubes.bin");
			fsh = myLoadShader("fs_cubes.bin");
			mProgram = bgfx::createProgram(vsh, fsh, true);
#endif
			*/
			return eOperationResult::Success;
		}

		void EndFrame()
		{
			// Send window resize event to the API thread.
			int oldWidth = mWidth, oldHeight = mHeight;
			glfwGetWindowSize(mWindow, (int*)&mWidth, (int*)&mHeight);
			if (mWidth != oldWidth || mHeight != oldHeight) {
				auto resize = new ResizeEvent;
				resize->width = (uint32_t)mWidth;
				resize->height = (uint32_t)mHeight;
				s_apiThreadEvents.push(resize);
			}

#ifdef _QDEARIMGUI
			// #TODO Fix bigg and bgfx imgui functionality
			// imguiEvents(dt);
			// ImGui::NewFrame();
			// ImGui::Render();
			// imguiRender(ImGui::GetDrawData());
#endif

#ifdef _QGLM
			s_ElapsedTime += Time::FrameDelta();

			glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -35.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(mWidth) / mHeight, .1f, 100.f);
#ifdef _QBGFX
			bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
			bgfx::setViewRect(0, 0, 0, uint16_t(mWidth), uint16_t(mHeight));
			// bgfx::touch(0);
			//
			// for (uint32_t yy = 0; yy < 11; ++yy)
			// {
			// 	for (uint32_t xx = 0; xx < 11; ++xx)
			// 	{
			// 		glm::mat4 mtx = glm::mat4(1.f);
			// 		mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 0.0f));
			// 		mtx *= glm::yawPitchRoll(s_ElapsedTime + xx * 0.21f, s_ElapsedTime + yy * 0.37f, 0.0f);
			// 		bgfx::setTransform(&mtx[0][0]);
			// 		bgfx::setVertexBuffer(0, mVbh);
			// 		bgfx::setIndexBuffer(mIbh);
			// 		bgfx::setState(BGFX_STATE_DEFAULT);
			// 		bgfx::submit(0, mProgram);
			// 	}
			// }
			//
			// bgfx::frame();
			// bgfx::renderFrame();
#endif
#endif
		}

		void Shutdown()
		{
#ifdef _QBGFX
			// bgfx::shutdown();
			s_ApiThread.shutdown();
#endif

#ifdef _QDEARIMGUI
			// imguiShutdown(); // From bigg library
#endif
		}

	}

}
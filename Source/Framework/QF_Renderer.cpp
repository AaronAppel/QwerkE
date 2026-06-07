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
#include "bgfxFramework/imguiCommon/imgui_impl_bgfx.h"
#include "Libraries/imgui/backends/imgui_impl_glfw.h"
#endif // _QDEARIMGUI

#endif // _QBGFXFRAMEWORK
#endif // _QBGFX

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#include "Libraries/glfw/glfw3native.h"
#endif // _QGLFW3

#include "QF_Enums.h"
#include "QF_Window.h"

namespace QwerkE::Renderer {

static bool sShowRendererDebugStats = false;

#ifdef _QBGFX // #TODO Move library specific code to some QF_Renderer_XXXX file

struct ViewportData {
	GLFWwindow* window = nullptr;
	bgfx::FrameBufferHandle framebuffer = BGFX_INVALID_HANDLE;
	// #TODO Use: FrameBuffer framebuffer;
	uint16_t viewId = 0;

	uint16_t width = 0;
	uint16_t height = 0;
};

// #TODO Look at using 0 or back buffer to catch poor renderer state, rendering to back buffer (possibly after bgfx::reset)
// static const bgfx::ViewId s_ViewIdBackBuffer = 0;

static const bgfx::ViewId sViewIdMain = 0;
static const bgfx::ViewId sViewIdImGui = 1;
static bgfx::ViewId sNextViewId = sViewIdImGui + 1;
static u16 sViewIdMax = 100;
#ifdef QE_DEBUG
static DebugDrawEncoder* sDebugDrawer = nullptr;
#endif

// Multi viewport function callbacks
static void _CreateWindow(ImGuiViewport* viewport); // #TOSO Fix CreateWindow name collision
static void DestroyWindow(ImGuiViewport* viewport);
static void SetWindowPos(ImGuiViewport* viewport, ImVec2 pos);
static ImVec2 GetWindowPos(ImGuiViewport* viewport);
static void SetWindowSize(ImGuiViewport* viewport, ImVec2 size);
static ImVec2 GetWindowSize(ImGuiViewport* viewport);
static void RenderWindow(ImGuiViewport* viewport, void*);
static void SwapBuffers(ImGuiViewport*, void*);
static void RecreateFramebuffer(ImGuiViewport* viewport);
#endif // _QBGFX

void OnWindowResized(u32 newWidth, u32 newHeight) {
	// assert(newWidth > 0 && newHeight > 0);
	if (1 > newWidth || 1 > newHeight) {
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

	for (size_t i = 0; i < sNextViewId - 1; i++) {
		bgfx::setViewRect(i, 0, 0, newWidth, newHeight);
		bgfx::setViewFrameBuffer(i, BGFX_INVALID_HANDLE);

		// DEBUG Change clear color to red to better catch writing to the back buffer
		// bgfx::setViewClear(i,
		// 	BGFX_CLEAR_COLOR,
		// 	0xff0000ff, 1.0f, 0);
	}
#endif
}

eOperationResult Initialize() {
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

	bgfx::setViewClear(sViewIdMain
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	bgfx::setViewClear(sViewIdImGui
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);
#endif
	const vec2f& size = Window::GetSize();
	OnWindowResized(size.x, size.y);

#ifdef QE_DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

#ifdef _QBGFX
#ifdef _QDEARIMGUI
	const float fontSize = 18.f;
	imguiCreate(fontSize);

	ImGui_ImplGlfw_InitForOther(window, true);
	ImGui_Implbgfx_Init(sViewIdImGui);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowSize.x, windowSize.y);
	// io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

	// #TODO Review ImGui styling
	// ImGuiStyle& style = ImGui::GetStyle();
	// style.WindowRounding = 0.0f;
	// style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	// Register viewport platform backend
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	platform_io.Platform_CreateWindow = _CreateWindow;
	platform_io.Platform_DestroyWindow = DestroyWindow;
	platform_io.Platform_SetWindowPos = SetWindowPos;
	platform_io.Platform_GetWindowPos = GetWindowPos;
	platform_io.Platform_SetWindowSize = SetWindowSize;
	platform_io.Platform_GetWindowSize = GetWindowSize;
	platform_io.Platform_RenderWindow = RenderWindow;
	platform_io.Platform_SwapBuffers = SwapBuffers;
#ifdef QE_DEBUG
	ddInit();
	sDebugDrawer = new DebugDrawEncoder();
#endif
#endif
	bgfx::touch(sViewIdMain); // Render main view 1st frame
#endif

	return eOperationResult::Success;
}

void Shutdown() {
	if (Window::IsMinimized())
		return; // #TODO Review shutdown while minimized

#ifdef _QBGFX
#ifdef _QDEARIMGUI

			// #TODO Shutdown multi viewport properly
			// ImGui_Implbgfx_Shutdown();
			// ImGui_ImplGlfw_Shutdown();

	imguiDestroy();
#endif
	ddShutdown();

	bgfx::setGraphicsDebuggerPresent(true); //#TODO Investigate debug break
	bgfx::shutdown();
#endif

#ifdef QE_DEBUG
	delete sDebugDrawer;
#endif
}

#if _QDEARIMGUI
void StartImGui() {
	GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	const vec2f& windowSize = Window::GetSize();

	// #TODO Review StartFrame() logic IF main view needs constant updating
	// bgfx::setViewRect(s_ViewIdMain, 0, 0, windowSize.x, windowSize.y);

	imguiBeginFrame(
		x, y
		, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1 ? IMGUI_MBUT_LEFT : 0
		// #TODO warning C4554: '|': check operator precedence for possible error; use parentheses to clarify precedence
		| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1 ? IMGUI_MBUT_RIGHT : 0
		// #TODO warning C4554: '|': check operator precedence for possible error; use parentheses to clarify precedence
		| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1 ? IMGUI_MBUT_MIDDLE : 0
		, 0
		, u16(windowSize.x)
		, u16(windowSize.y)
		, -1
		, sViewIdImGui
	);

	bgfx::touch(sViewIdImGui);
}

void EndImGui() {
	imguiEndFrame();
}
#endif // _QDEARIMGUI

void EndFrame() {
	if (Window::IsMinimized())
		return;

#ifdef _QBGFX
	bgfx::frame();

#ifdef QE_DEBUG
	bgfx::setDebug(sShowRendererDebugStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
	bgfx::dbgTextClear();
#endif
#endif
}

u16 NextViewId() {
	ASSERT(sNextViewId < sViewIdMax, "ViewId limit reached!");
	return sNextViewId++;
}

#ifdef QE_DEBUG
void ToggleDebugStats() {
	sShowRendererDebugStats = !sShowRendererDebugStats;
}

DebugDrawEncoder& DebugDrawer() {
	ASSERT(sDebugDrawer, "Debug Drawer is null!");
	return *sDebugDrawer;
}
#endif

#if _QBGFX
		// Multi viewport function callbacks

		// #TODO Improve naming of all functions, but especially this one abused by Windows
static void _CreateWindow(ImGuiViewport* viewport) {
	GLFWwindow* window = glfwCreateWindow(
		(int)viewport->Size.x,
		(int)viewport->Size.y,
		"ImGui Viewport",
		nullptr,
		nullptr
	);

	HWND hwnd = glfwGetWin32Window(window);

	auto* data = new ViewportData();
	data->window = window;
	data->viewId = NextViewId();

	viewport->PlatformUserData = data;
	viewport->PlatformHandle = window;

	RecreateFramebuffer(viewport);
}

static void DestroyWindow(ImGuiViewport* viewport) {
	auto* data = (ViewportData*)viewport->PlatformUserData;
	if (!data)
		return;

	if (bgfx::isValid(data->framebuffer))
		bgfx::destroy(data->framebuffer);

	glfwDestroyWindow(data->window);
	delete data;

	viewport->PlatformUserData = nullptr;
	viewport->PlatformHandle = nullptr;
}

static void SetWindowPos(ImGuiViewport* viewport, ImVec2 pos) {
	auto* data = (ViewportData*)viewport->PlatformUserData;
	glfwSetWindowPos(data->window, (int)pos.x, (int)pos.y);
}

static ImVec2 GetWindowPos(ImGuiViewport* viewport) {
	auto* data = (ViewportData*)viewport->PlatformUserData;
	int x, y;
	glfwGetWindowPos(data->window, &x, &y);
	return ImVec2((float)x, (float)y);
}

static void SetWindowSize(ImGuiViewport* viewport, ImVec2 size) {
	auto* data = (ViewportData*)viewport->PlatformUserData;
	glfwSetWindowSize(data->window, (int)size.x, (int)size.y);
}

static ImVec2 GetWindowSize(ImGuiViewport* viewport) {
	auto* data = (ViewportData*)viewport->PlatformUserData;
	int w, h;
	glfwGetWindowSize(data->window, &w, &h);
	return ImVec2((float)w, (float)h);
}

static void RenderWindow(ImGuiViewport* viewport, void*) {
	ViewportData* viewportData = (ViewportData*)viewport->PlatformUserData;

	RecreateFramebuffer(viewport);

	if (!bgfx::isValid(viewportData->framebuffer))
		return;

	// #TODO Review framebuffer data
	bgfx::setViewFrameBuffer(viewportData->viewId, viewportData->framebuffer);
	bgfx::setViewRect(
		viewportData->viewId,
		0, 0,
		viewportData->width,
		viewportData->height
	);

	bgfx::setViewClear(
		viewportData->viewId,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x2b2b2bff
	);

	bgfx::touch(viewportData->viewId);
}

static void SwapBuffers(ImGuiViewport*, void*) {
	// GLFW handles swap internally per-window
}

static bool IsWindowMinimized(GLFWwindow* w) {
	int iconified = glfwGetWindowAttrib(w, GLFW_ICONIFIED);
	return iconified == GLFW_TRUE;
}

static void RecreateFramebuffer(ImGuiViewport* viewport) {
	auto* data = (ViewportData*)viewport->PlatformUserData;

	if (IsWindowMinimized(data->window))
		return;

	// Why glfwGetFramebufferSize instead of GetWindowSize ?
	// - Correct for DPI scaling
	// - Matches what bgfx expects
	int w, h;
	glfwGetFramebufferSize(data->window, &w, &h);

	if (w <= 0 || h <= 0)
		return; // minimized or invalid

	uint16_t newW = (uint16_t)w;
	uint16_t newH = (uint16_t)h;

	// No change? Do nothing.
	if (data->width == newW && data->height == newH)
		return;

	// Destroy old framebuffer
	if (bgfx::isValid(data->framebuffer))
		bgfx::destroy(data->framebuffer);

	HWND hwnd = glfwGetWin32Window(data->window);

	data->framebuffer = bgfx::createFrameBuffer(hwnd, newW, newH);

	data->width = newW;
	data->height = newH;
}
#endif // _QBGFX

}

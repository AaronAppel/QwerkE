#include "Framework.h"

#include <assert.h>
#include <iostream>

#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QBGFX
#include <bx/timer.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#endif

#ifdef _QBGFXFRAMEWORK
#include "bgfxFramework/bgfx_utils.h"
#include "bgfxFramework/bgfx_utils.h"
#include "bgfxFramework/entry/bgfxFrameworkEntry.h"
#include "bgfxFramework/debugDraw/debugdraw.h"
#include "bgfxFramework/imguiCommon/imguiCommon.h"
// #include "bgfxFramework/entry/entry.h"
#include "bgfxFramework/camera.h" // For keys macros
#endif

#include "Camera.h"
#include "bgfxFramework/LoadShader.h"
#include "Renderer.h"
#include "Window.h"

namespace Framework {

	void Run(unsigned int argc, char** argv)
	{
		float windowWidth = 1600, windowHeight = 900;

		Window::Initialize(windowWidth, windowHeight);
		Renderer::Initialize();

		GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());

		entry::Initialize();

		Mesh* mesh = meshLoad("meshes/bunny.bin");
		assert(mesh && "Mesh is null!");

		const bgfx::ShaderHandle vsh = myLoadShader("vs_cubes.bin");
		const bgfx::ShaderHandle fsh = myLoadShader("fs_cubes.bin");
		const bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

		const bgfx::UniformHandle u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4); // Dealloc?

		const int64_t m_StartingTimeOffset = bx::getHPCounter();

		DebugDrawEncoder debugDrawer;

		Camera camera;

		entry::MouseState mouseState;

		while (!Window::CloseRequested())
		{
			Window::NewFrame();

			if (Window::IsMinimized())
			{
				continue;
			}

			if (glfwGetKey(window, GLFW_KEY_F1))
			{
				Renderer::ToggleDebugStats();
			}

			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			{
				Window::RequestClose();
			}
			else
			{
				const float deltaTime = (float)((bx::getHPCounter() - m_StartingTimeOffset) / double(bx::getHPFrequency()));

				double x, y;
				glfwGetCursorPos(window, &x, &y);

				{	// imgui
					imguiBeginFrame(x
						, y
						, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1 ? IMGUI_MBUT_LEFT : 0
						| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1 ? IMGUI_MBUT_RIGHT : 0
						| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1 ? IMGUI_MBUT_MIDDLE : 0
						, 0
						, uint16_t(windowWidth)
						, uint16_t(windowHeight)
					);

					static bool s_ShowingImGuiExampleWindow = false;
					if (ImGui::BeginMainMenuBar())
					{
						if (ImGui::BeginMenu("Menu"))
						{
							static int index = 0;
							static const int size = 1;
							const char* d[size] = { "ExampleWindow" };
							if (ImGui::ListBox("MenuListLabel", &index, d, size, 3))
							{
								if (index == 0) s_ShowingImGuiExampleWindow = !s_ShowingImGuiExampleWindow;
							}
							ImGui::EndMenu();
						}

						ImGui::EndMainMenuBar();
					}

					if (s_ShowingImGuiExampleWindow) ImGui::ShowDemoWindow(&s_ShowingImGuiExampleWindow);

					static bool dockOpen = true;
					// ImGui::BeginDock("Dock Label", &dockOpen, 0);

					ImGui::Begin("Test Window");

					bx::Vec3 tempEye = camera.GetEye();
					if (ImGui::DragFloat3("Eye", &tempEye.x, .1f))
					{
						camera.SetEye(tempEye);
					}
					ImGui::End();

					// ImGui::EndDock();

					imguiEndFrame();
				}

				{	// Update camera.
					// const float deltaTime = Time::FrameDelta();

					// const bool leftMouseIsDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1;
					// mouseState.m_buttons[entry::MouseButton::Left] = leftMouseIsDown;

					const bool forwardCameraIsDown = glfwGetKey(window, GLFW_KEY_W) == 1;
					camera.SetKeyState(CAMERA_KEY_FORWARD, forwardCameraIsDown);

					camera.Update(deltaTime, mouseState, ImGui::MouseOverArea());

					float view[16];
					camera.GetViewMtx(view);

					float proj[16];

					// Set view and projection matrix for view 0.
					{
						bx::mtxProj(proj, 60.0f, float(windowWidth) / float(windowHeight), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

						bgfx::setViewTransform(0, view, proj);
						bgfx::setViewRect(0, 0, 0, uint16_t(windowWidth), uint16_t(windowHeight));
					}

					float mtxVp[16];
					bx::mtxMul(mtxVp, view, proj);

					float mtxInvVp[16];
					bx::mtxInverse(mtxInvVp, mtxVp);

					const bx::Vec3 at = { 0.0f,  0.0f, 0.0f };
					bx::mtxLookAt(view, camera.GetEye(), at);
					bx::mtxProj(proj, 45.0f, float(windowWidth) / float(windowHeight), 1.0f, 15.0f, bgfx::getCaps()->homogeneousDepth);
					bx::mtxMul(mtxVp, view, proj);
				}

				if (mesh)
				{
					debugDrawer.begin(0);

					{
						const bx::Vec3 normal = { 0.f,  1.f, 0.f };
						const bx::Vec3 pos = { 0.f, -2.f, 0.f };

						bx::Plane plane(bx::InitNone);
						bx::calcPlane(plane, normal, pos);

						//debugDrawer.setColor(false
						//	|| intersect(&debugDrawer, ray, plane)
						//	? kSelected
						//	: 0xffffffff
						//);

						debugDrawer.drawGrid(Axis::Y, pos, 128, 1.0f);
					}

					debugDrawer.end();

					// bgfx::setViewRect(0, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y)); // Set view 0 default viewport.

					bgfx::touch(0); // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.

					bgfx::setUniform(u_time, &deltaTime);

					//const bx::Vec3 at = { 0.0f, 1.0f,  0.0f };
					//const bx::Vec3 eye = { 0.0f, 1.0f, -2.5f };
					//
					//// Set view and projection matrix for view 0.
					//{
					//	float view[16];
					//	bx::mtxLookAt(view, eye, at);
					//
					//	float proj[16];
					//	bx::mtxProj(proj, 60.0f, float(windowSize.x) / float(windowSize.y), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
					//	bgfx::setViewTransform(0, view, proj);
					//
					//	// Set view 0 default viewport.
					//	bgfx::setViewRect(0, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));
					//}

					float mtx[16];

					bx::mtxTranslate(mtx, 0.f, -2.f, 0.f);

					bx::mtxRotateXY(mtx
						, 0.0f
						, deltaTime * 0.37f
					);

					bx::mtxScale(mtx, 15.f);

					meshSubmit(mesh, 0, program, mtx);
				}
				else
				{
					bgfx::dbgTextPrintf(0, 3, 0x0f, "Null mesh");
				}
			}

			Renderer::EndFrame();
		}

		bgfx::destroy(program);
		meshUnload(mesh);

		entry::Shutdown();

		Renderer::Shutdown();
		Window::Shutdown();
	}

}

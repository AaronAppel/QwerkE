#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Scene.h"
#include "QF_Scenes.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	class Scene;

	namespace Editor {

		struct EditorCamera // #TODO Refactor
		{
			void PreDrawSetup(bgfx::ViewId viewId)
			{
				const vec2f& windowSize = Window::GetSize();

				bgfx::setViewRect(viewId, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

				bx::mtxLookAt(m_View, m_Eye, m_LookAtTarget);

				bx::mtxProj(m_Proj, m_Fov, windowSize.x / windowSize.y, m_Near, m_Far, bgfx::getCaps()->homogeneousDepth);
				bgfx::setViewTransform(viewId, m_View, m_Proj);

				if (m_ShowSphere)
				{
					DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
					debugDrawer.begin(viewId);
					debugDrawer.drawSphere(m_LookAtTarget.x, m_LookAtTarget.y, m_LookAtTarget.z, 3.f);
					debugDrawer.end();
				}
			}

			bool m_ShowSphere = false;
			bx::Vec3 m_Eye = bx::Vec3(0.f);
			bx::Vec3 m_LookAtTarget = bx::Vec3(0.f);;

			float m_Fov = 60.f;
			float m_Near = .1f;
			float m_Far = 100.f;

			float m_View[16];
			float m_Proj[16];
		};

		class EditorWindowSceneView : public EditorWindow
		{
		public:
			EditorWindowSceneView::EditorWindowSceneView(u8 textureId, u8 viewId) :
				EditorWindow("Scene View", EditorWindowTypes::SceneView),
				m_TextureId(textureId),
				m_ViewId(viewId)
			{
				m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
				snprintf(m_ScenesCombobuffer, sizeof(m_ScenesCombobuffer), "Scenes: %i##%llu", 0, GetGuid());
			}

			EditorWindowSceneView::EditorWindowSceneView(GUID guid = GUID()) :
				EditorWindow("Scene View", EditorWindowTypes::SceneView, guid)
			{
				m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
			}

			void SetTextureId(u8 textureId) { m_TextureId = textureId; }
			void SetViewId(u8 viewId) { m_ViewId = viewId; }

		private:
			void DrawInternal()
			{
				if (nullptr == m_CurrentScene)
				{
					m_CurrentScene = Scenes::GetCurrentScene();
					m_LastSceneIndex = Scenes::GetCurrentSceneIndex();
				}

				const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
				if (!scenes.empty())
				{
					std::vector<const char*> sceneNames;
					sceneNames.reserve(3);

					for (size_t i = 0; i < scenes.size(); i++)
					{
						sceneNames.push_back(scenes[i]->GetSceneName().c_str());
					}

					constexpr u32 s_CharacterPixelSize = 10;
					constexpr u32 s_DropDownArrowSize = 20;

					const u32 sceneFileNameWidth = (u32)strlen(sceneNames[m_LastSceneIndex]) * s_CharacterPixelSize;

					ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);

					snprintf(m_ScenesCombobuffer, sizeof(m_ScenesCombobuffer), "Scenes: %i##%llu", (int)sceneNames.size(), GetGuid());

					ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(m_ScenesCombobuffer) * s_CharacterPixelSize));
					if (ImGui::Combo(m_ScenesCombobuffer, &m_LastSceneIndex, sceneNames.data(), (s32)scenes.size()))
					{
						m_CurrentScene = scenes[m_LastSceneIndex];
						// #NOTE Scene transition changes removes above lines for below
						// #TODO SetActive(true/false) ?
						// Scenes::SetCurrentScene(index);
					}
					ImGui::PopItemWidth();
				}
				else
				{
					ImGui::Text("No scene selected");
				}

				if (!m_CurrentScene)

				// #NOTE Scene transition changes
				// constexpr u32 s_CharacterPixelSize = 10;
				// constexpr u32 s_DropDownArrowSize = 20;
				//
				// const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
				// if (!scenes.empty())
				// {
				// 	std::vector<const char*> sceneNames;
				// 	sceneNames.reserve(3);
				//
				// 	int index = 0;
				// 	for (size_t i = 0; i < scenes.size(); i++)
				// 	{
				// 		sceneNames.push_back(scenes[i]->GetSceneName().c_str());
				// 		if (scenes[i]->GetGuid() == m_SelectedSceneGuid)
				// 		{
				// 			index = i;
				// 		}
				// 	}
				//
				// 	const u32 sceneFileNameWidth = (u32)strlen(sceneNames[index]) * s_CharacterPixelSize;
				//
				// 	ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);
				//
				// 	char s_ScenesCombobuffer[] = "Scenes:    ";
				// 	snprintf(s_ScenesCombobuffer, strlen(s_ScenesCombobuffer), "Scenes: %i", (int)sceneNames.size());
				//
				// 	// #TODO Use ImGui::SameLineEnd();
				// 	ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ScenesCombobuffer) * s_CharacterPixelSize));
				// 	if (ImGui::Combo(s_ScenesCombobuffer, &index, sceneNames.data(), (s32)scenes.size()))
				// 	{
				// 		// #TODO SetActive(true/false) ?
				// 		m_SelectedSceneGuid = index;
				// 	}
				// 	ImGui::PopItemWidth();
				// }
				// else
				// {
				// 	ImGui::SameLineEnd("Scenes : 0");
				// 	ImGui::Text("Scenes : 0");
				// }
				//
				// Scene* sceneToView = Scenes::GetScene(m_SelectedSceneGuid);
				// if (!sceneToView)
					return;

				// #NOTE Scene transition changes removed 2 lines below


				// #TODO Buttons are squares, so maybe width = (lineheight * 3) + (padding * 2)
				// x3 for the 2 buttons and field, plus the 2 spaces in between the 3 buttons
				constexpr float itemWidth = 80.f;

				ImGui::PushItemWidth(itemWidth);
				s32 tempIntViewId = m_ViewId;
				if (ImGui::InputInt("View Id", &tempIntViewId, 1))
				{
					constexpr s32 maxViewId = 4;
					m_ViewId = Math::Clamp(0, tempIntViewId, maxViewId);
				}
				ImGui::PopItemWidth();

				ImGui::PushItemWidth(itemWidth);
				s32 tempIntTextureId = m_TextureId;
				ImGui::SameLine();
				if (ImGui::InputInt("Texture Id", &tempIntTextureId, 1))
				{
					constexpr s32 maxTextureId = 4;
					m_TextureId = Math::Clamp(0, tempIntTextureId, maxTextureId);
				}
				ImGui::PopItemWidth();

				// #TODO Camera selection drop down
				std::vector<const char*> cameraEntityNames;
				static int currentCameraIndex = 0;

				auto viewCameras = m_CurrentScene->ViewComponents<ComponentCamera>();
				for (auto& entity : viewCameras)
				{
					EntityHandle handle(m_CurrentScene, entity);
					cameraEntityNames.push_back(handle.EntityName().c_str());
				}

				ImGui::PushItemWidth(120.f);
				ImGui::SameLineEnd(15);
				if (ImGui::Combo("Camera:", &currentCameraIndex, cameraEntityNames.data(), (int)cameraEntityNames.size()))
				{
					// #TODO Change camera. Can use name or entity GUID to find camera, for now
					for (auto& entity : viewCameras)
					{
						EntityHandle handle(m_CurrentScene, entity);
						if (handle.EntityName() == cameraEntityNames[currentCameraIndex])
						{
							Scenes::GetCurrentScene()->SetCurrentCameraEntity(handle);
						}
					}
				}
				ImGui::PopItemWidth();

				// #NOTE Scene transition changes
				// #TODO Draw grid
				// {
				// 	// Debug drawer calls
				// 	constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
				// 	bgfx::setState(BGFX_STATE_DEFAULT);
				// 	DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
				// 	debugDrawer.begin(viewIdFbo1, true);
				//
				// 	constexpr bx::Vec3 normal = { .0f,  1.f, .0f };
				// 	constexpr bx::Vec3 pos = { .0f, .0f, .0f };
				//
				// 	debugDrawer.drawSphere(0.f, 0.f, 0.f, 1.f, Axis::X);
				// 	// debugDrawer.drawOrb(0.f, 0.f, 0.f, 3.f, Axis::X);
				//
				// 	// #TODO Move grid draw to QE_EditorWindowSceneView draw call
				// 	bx::Plane plane(bx::InitNone);
				// 	bx::calcPlane(plane, normal, pos);
				//
				// 	debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);
				//
				// 	debugDrawer.end();
				// }

				// #TODO Draw scene using camera selected from Camera combo drop down
				m_EditorCamera.PreDrawSetup(m_ViewId);
				m_CurrentScene->Draw(m_ViewId);

				ImGui::Image(ImTextureID(m_TextureId), ImGui::GetContentRegionAvail(), ImVec2(0, 0), ImVec2(1, 1));
			}

			MIRROR_PRIVATE_MEMBERS

			EditorCamera m_EditorCamera;

			void OnSceneReload() override { m_CurrentScene = nullptr; }

			Scene* m_CurrentScene = nullptr;
			char m_ScenesCombobuffer[33] = "Scenes:    ##0000000000000000000";
			s32 m_LastSceneIndex = 0;

			// #TODO Scene transition changes
			// GUID m_SelectedSceneGuid;

			u8 m_TextureId = 3;
			u8 m_ViewId = 0;
		};

	}

}
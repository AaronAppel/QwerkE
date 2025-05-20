#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Time.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentTransform.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	class Scene;

	namespace Editor {

		class EditorWindowSceneView : public EditorWindow
		{
		public:
			EditorWindowSceneView(u8 textureId, u8 viewId) :
				EditorWindow("Scene View", EditorWindowTypes::SceneView),
				m_TextureId(textureId),
				m_ViewId(viewId)
			{
				m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
				snprintf(m_ScenesComboLabelBuffer, sizeof(m_ScenesComboLabelBuffer), "Scenes: %i##%llu", 0, GetGuid());
				m_EditorCamera.m_ShowSphere = false;
				// #TODO Improve construction of matrices
				float temp[16] = {
					0.99999862909317,
					0,
					-0.0016666054725646973,
					0,
					2.7775738544733031e-06,
					0.99999862909317,
					0.0016666031442582607,
					0,
					0.0016666031442582607,
					-0.0016666054725646973,
					0.99999725818634033,
					0,
					-4.401646614074707,
					3.784186840057373,
					-6.67338514328003,
					1
				};
				memcpy(m_EditorCameraTransform.m_Matrix, temp, sizeof(float) * 16);
			}

			EditorWindowSceneView::EditorWindowSceneView(GUID guid = GUID()) :
				EditorWindow("Scene View", EditorWindowTypes::SceneView, guid)
			{
				m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
				m_EditorCamera.m_ShowSphere = false;
				// #TODO Improve construction of matrices
				float temp[16] = {
					0.99999862909317,
					0,
					-0.0016666054725646973,
					0,
					2.7775738544733031e-06,
					0.99999862909317,
					0.0016666031442582607,
					0,
					0.0016666031442582607,
					-0.0016666054725646973,
					0.99999725818634033,
					0,
					-4.401646614074707,
					3.784186840057373,
					-6.67338514328003,
					1
				};
				memcpy(m_EditorCameraTransform.m_Matrix, temp, sizeof(float) * 16);
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
				if (scenes.empty())
				{
					ImGui::Text("No scene selected");
					return;
				}

				if (!m_CurrentScene)
				{
					ImGui::Text("Null scene selected");
					return;
				}

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

				snprintf(m_ScenesComboLabelBuffer, sizeof(m_ScenesComboLabelBuffer), "##Scenes: %i##%llu", (int)sceneNames.size(), GetGuid());

				// ImGui::Dummy(ImVec2(0, 0));
				// ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(m_ScenesComboLabelBuffer) * s_CharacterPixelSize));
				ImGui::SameLine();
				if (ImGui::Combo(m_ScenesComboLabelBuffer, &m_LastSceneIndex, sceneNames.data(), (s32)scenes.size()))
				{
					m_CurrentScene = scenes[m_LastSceneIndex];
					// #NOTE Scene transition changes removes above lines for below
					// #TODO SetActive(true/false) ?
					// Scenes::SetCurrentScene(index);
				}
				ImGui::PopItemWidth();

				ImGui::SameLine();
				ImGui::PushItemWidth(11 * ImGui::g_pixelsPerCharacter);
				// #TODO Game window (texture) scaling options
				int listIndex = 0;
				if (ImGui::Combo("Scaling", &listIndex,
					"Ratio\0"
					"Size\0"
					"Fit Window\0"
					"Pixel\0"
					))
				{
					// #TODO: Make Menu Ratios -> [16:9, 3:4, etc...]
					// #TODO: Make Menu Size -> [1024×576, 1280×720, etc..., Custom]
				}
				ImGui::PopItemWidth();

				// #TODO Camera selection drop down
				std::vector<const char*> cameraEntityNames;
				static int currentCameraIndex = 0; // #TODO needs to persist and adjust for newly added camera(s)

				auto viewCameras = m_CurrentScene->ViewComponents<ComponentCamera>();
				for (auto& entity : viewCameras)
				{
					EntityHandle handle(m_CurrentScene, entity);
					cameraEntityNames.push_back(handle.EntityName().c_str());
				}

				// ImGui::SameLineEnd(22);
				// ImGui::Text("Camera");
				ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::Combo("##Camera:", &currentCameraIndex, cameraEntityNames.data(), (int)cameraEntityNames.size()))
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

				if (ImGui::IsItemClicked(ImGui::Buttons::MouseRight))
				{
					ImGui::OpenPopup("CameraPerspective");
				}

				if (ImGui::BeginPopup("CameraPerspective"))
				{
					if (ImGui::MenuItem("Perspective"))
					{
						m_EditorCamera.m_Perspective = true;
					}
					if (ImGui::MenuItem("Orthographic"))
					{
						m_EditorCamera.m_Perspective = false;
					}
					ImGui::EndPopup();
				}

				// #TODO Draw scene using camera selected from Camera combo drop down
				if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
				{
					EditorCameraUpdate();
				}
				m_EditorCamera.PreDrawSetup(m_ViewId, m_EditorCameraTransform.GetPosition());
				m_CurrentScene->Draw(m_EditorCamera, m_EditorCameraTransform.GetPosition(), m_ViewId);

				ImGui::Image(ImTextureID(m_TextureId), ImGui::GetContentRegionAvail(), ImVec2(0, 0), ImVec2(1, 1));
			}

			void EditorCameraUpdate()
			{
				const float deltaTime = Time::PreviousFrameDuration();

				static float pixelRatio = 5.f; // #TODO Review name and purpose. Higher values mean slower camera movement

				if (Input::MouseDown(QKey::e_MouseRight))
				{
					static float yaw = 0.f;
					yaw += Input::MouseDelta().x / pixelRatio * deltaTime;

					// Pitch transform.m_Matrix[6];
					static float pitch = 0.f;
					pitch += Input::MouseDelta().y / pixelRatio * deltaTime;

					constexpr bx::Vec3 scale = { 1.f, 1.f, 1.f };
					bx::Vec3 rotate = { pitch, yaw, 0.f };
					const vec3f& translate = m_EditorCameraTransform.GetPosition();

					bx::mtxSRT(m_EditorCameraTransform.m_Matrix,
						scale.x, scale.y, scale.z,
						rotate.x, rotate.y, rotate.z,
						translate.x, translate.y, translate.z);
				}

				const vec3f transformForward = m_EditorCameraTransform.Forward(); // #TODO Calculate and use proper forward
				const bx::Vec3 forward =
				{
					transformForward.x,
					transformForward.y,
					transformForward.z
				};

				// #TODO Review how hotkeys are managed by framework and customized by game

				const Input::GameActions& gameActions = Input::GetGameActions();

				if (Input::KeyDown(gameActions.Camera_MoveForward) ||
					Input::GamepadDown(QGamepad::e_GamepadA))
				{
					vec3f pos = m_EditorCameraTransform.GetPosition();
					bx::Vec3 eye = bx::mad(forward, deltaTime * m_EditorCamera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
					m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
					// m_EditorCameraTransform.m_Matrix[14] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}
				if (Input::KeyDown(gameActions.Camera_MoveBackward))
				{
					vec3f pos = m_EditorCameraTransform.GetPosition();
					bx::Vec3 eye = bx::mad(forward, -deltaTime * m_EditorCamera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
					m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
					// m_EditorCameraTransform.m_Matrix[14] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}

				const bx::Vec3 right =
				{
					m_EditorCameraTransform.m_Matrix[0],
					m_EditorCameraTransform.m_Matrix[4],
					m_EditorCameraTransform.m_Matrix[8]
				};

				if (Input::KeyDown(gameActions.Camera_MoveLeft))
				{
					vec3f pos = m_EditorCameraTransform.GetPosition();
					bx::Vec3 eye = bx::mad(right, -deltaTime * m_EditorCamera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
					m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
					// m_EditorCameraTransform.m_Matrix[12] -= (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}
				if (Input::KeyDown(gameActions.Camera_MoveRight))
				{
					vec3f pos = m_EditorCameraTransform.GetPosition();
					bx::Vec3 eye = bx::mad(right, deltaTime * m_EditorCamera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
					m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
					// m_EditorCameraTransform.m_Matrix[12] += (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}

				const bx::Vec3 up = bx::cross(right, forward);

				if (Input::KeyDown(gameActions.Camera_MoveDown))
				{
					m_EditorCameraTransform.m_Matrix[13] -= (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}
				if (Input::KeyDown(gameActions.Camera_MoveUp))
				{
					m_EditorCameraTransform.m_Matrix[13] += (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
				}
				if (Input::KeyDown(gameActions.Camera_RotateRight))
				{
					// LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
				}
				if (Input::KeyDown(gameActions.Camera_RotateLeft))
				{
					constexpr float rotationSpeed = Math::PI_f();
					bx::mtxRotateXYZ(m_EditorCameraTransform.m_Matrix, 0.f, rotationSpeed * deltaTime, 0.f);
					// LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
				}

				const float mouseScroll = Input::MouseScrollDelta();
				if (mouseScroll != 0.f)
				{
					m_EditorCamera.m_Fov -= mouseScroll;
				}

				if (const bool useTargetLookAt = false)
				{
					// #TODO Add option to reference an existing transform component in the scene
					vec3f targetPosition = m_EditorCamera.m_LookAtPosition;
					m_EditorCamera.m_LookAtPosition = targetPosition;
				}
				else if (const bool useDirectionalLookAt = true)
				{
					// #TODO Only re-calculate if forward changed
					constexpr float scalar = 1.f;
					const float* position = nullptr;
					vec3f forwardPosition = m_EditorCameraTransform.GetPosition() + (m_EditorCameraTransform.Forward() * scalar);
					m_EditorCamera.m_LookAtPosition = forwardPosition;
				}
			}

			MIRROR_PRIVATE_MEMBERS

			ComponentCamera m_EditorCamera;
			ComponentTransform m_EditorCameraTransform;
			Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(QKey::e_MouseButton2);

			void OnSceneReload() override { m_CurrentScene = nullptr; }

			Scene* m_CurrentScene = nullptr;
			char m_ScenesComboLabelBuffer[33] = "Scenes:    ##0000000000000000000";
			s32 m_LastSceneIndex = 0;

			// #TODO Scene transition changes
			// GUID m_SelectedSceneGuid;

			u8 m_TextureId = 3;
			u8 m_ViewId = 0;
		};

	}

}
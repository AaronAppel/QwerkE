#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

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

				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
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
				Scene* currentScene = Scenes::GetCurrentScene();
				if (!currentScene)
					return;

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

				{	// Draw scene using camera
					m_EditorCamera.PreDrawSetup(m_ViewId);
					currentScene->Draw(m_ViewId);
				}

				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
				ImGui::Image(ImTextureID(m_TextureId), ImVec2(contentRegionAvailable.x, contentRegionAvailable.y), ImVec2(0, 1), ImVec2(1, 0));
			}

			MIRROR_PRIVATE_MEMBERS

			EditorCamera m_EditorCamera;

			u8 m_TextureId = 3;
			u8 m_ViewId = 0;
		};

	}

}